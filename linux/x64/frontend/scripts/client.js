// Static metadata — no server needed to populate dropdowns
const PRODUCT_TYPES = [
    'omnihand_2025',
    'omnihand_pro_2025',
    'omnihand_dex_umi',
    'omnihand_3_lite',
    'omnihand_3_ultra_m',
];

const CONN_TYPES = [
    'zlgcan', 'hcan', 'socketcan', 'zlgcan_tcp', 'tj', 'rs485', 'usb',
];

const CONN_CONFIG_SCHEMA = {
    zlgcan: [
        { canfd_device_id: 'int', canfd_channel_id: 'int' },
        { usbcanfd_serial_number: 'string', canfd_channel_id: 'int' },
    ],
    hcan: [
        { canfd_device_id: 'int', canfd_channel_id: 'int' },
        { hcan_serial_number: 'string', canfd_channel_id: 'int' },
    ],
    socketcan: [{ can_interface: 'string' }],
    zlgcan_tcp: [{ host: 'string', port: 'int', canfd_channel_id: 'int' }],
    tj: [{ marvin_controller_ip: 'string' }],
    rs485: [{ uart_port: 'string', baudrate: 'int' }],
    usb: [{ uart_port: 'string' }],
};

const DEFAULT_API_BASE_URL = 'http://127.0.0.1:8000';
const WS_REQUEST_TIMEOUT_MS = 10000;
const WS_STREAMS = ['state', 'diagnostics', 'tactile'];
const WS_CHART_IDS = {
    state: 'ws-state-chart',
    diagnostics: 'ws-diagnostics-chart',
    tactile: 'ws-tactile-chart',
};

let configHost = DEFAULT_API_BASE_URL;
let handsCache = [];
const methodsCache = {};
const wsCharts = {
    state: null,
    diagnostics: null,
    tactile: null,
};

const wsClient = {
    socket: null,
    requestId: 0,
    pending: new Map(),
    connected: false,
    methodsCache: {},
};

function normalizeApiBaseUrl(value) {
    const raw = String(value || DEFAULT_API_BASE_URL).trim();
    const withProtocol = /^https?:\/\//i.test(raw) ? raw : `http://${raw}`;
    return withProtocol.replace(/\/+$/, '');
}

async function loadRuntimeConfig() {
    try {
        const response = await fetch('./configs/config.json');
        if (!response.ok) {
            throw new Error(`config.json load failed: ${response.status}`);
        }
        const config = await response.json();
        configHost = normalizeApiBaseUrl(config?.apiBaseUrl);

        const input = document.getElementById('server-host-input');
        if (input) {
            input.value = configHost;
        }
    } catch (error) {
        console.warn('Using default API base URL.', error);
        configHost = normalizeApiBaseUrl(DEFAULT_API_BASE_URL);
    }
}

function getServerBase() {
    const input = document.getElementById('server-host-input');
    if (input) {
        const val = input.value.trim();
        if (val) return normalizeApiBaseUrl(val);
    }
    return configHost;
}

const urlBuilder = (path) => {
    const base = getServerBase();
    const normalizedPath = String(path).replace(/^\/+/, '');
    return `${base}/${normalizedPath}`;
};

function buildWsUrl(path = '/ws') {
    const url = new URL(getServerBase());
    url.protocol = url.protocol === 'https:' ? 'wss:' : 'ws:';
    url.pathname = path.startsWith('/') ? path : `/${path}`;
    url.search = '';
    url.hash = '';
    return url.toString();
}

loadRuntimeConfig();

function setText(id, value) {
    const el = document.getElementById(id);
    if (el) {
        el.textContent = value;
    }
}

function prettyJson(value) {
    return JSON.stringify(value, null, 2);
}

function handLabel(hand) {
    const type = hand.hand_type || hand.product_type || 'unknown';
    return `${hand.hand_id} (${type})`;
}

function setSelectPlaceholder(select, text) {
    select.innerHTML = '';
    const option = document.createElement('option');
    option.value = '';
    option.textContent = text;
    option.disabled = true;
    option.selected = true;
    select.appendChild(option);
}

function populateHandSelect(selectId, hands, placeholder) {
    const select = document.getElementById(selectId);
    if (!select) return;
    const previousValue = select.value;
    setSelectPlaceholder(select, placeholder);
    hands.forEach((hand) => {
        const option = document.createElement('option');
        option.value = hand.hand_id;
        option.textContent = handLabel(hand);
        select.appendChild(option);
    });
    if (hands.some((hand) => String(hand.hand_id) === previousValue)) {
        select.value = previousValue;
    }
}

function populateMethodQueryHandSelect(hands) {
    populateHandSelect('hand-id-for-method-select', hands, '-- Select a hand --');
}

function populateDeleteHandSelect(hands) {
    populateHandSelect('hand-id-input', hands, '-- Select hand ID --');
}

function populateWsHandSelect(hands) {
    [
        'ws-hand-select',
        'ws-method-hand-select',
        'ws-state-hand-select',
        'ws-diagnostics-hand-select',
        'ws-tactile-hand-select',
    ].forEach((selectId) => {
        populateHandSelect(selectId, hands, '-- Select hand ID --');
    });
}

function updateHandsCache(hands) {
    handsCache = Array.isArray(hands) ? hands : [];
    populateDeleteHandSelect(handsCache);
    populateMethodCallHandSelect(handsCache);
    populateMethodQueryHandSelect(handsCache);
    populateWsHandSelect(handsCache);
}


function populateProductTypeSelects() {
    const ptPlaceholder = document.createElement('option');
    ptPlaceholder.value = '';
    ptPlaceholder.textContent = '-- Select product type --';
    ptPlaceholder.disabled = true;
    ptPlaceholder.selected = true;

    const productTypeSelect = document.getElementById('product-type-select');
    productTypeSelect.appendChild(ptPlaceholder.cloneNode(true));
    PRODUCT_TYPES.forEach((type) => {
        const option = document.createElement('option');
        option.value = type;
        option.textContent = type;
        productTypeSelect.appendChild(option);
    });

    const productTypeForMethodSelect = document.getElementById('product-type-for-method-select');
    productTypeForMethodSelect.innerHTML = '';
    productTypeForMethodSelect.appendChild(ptPlaceholder.cloneNode(true));
    PRODUCT_TYPES.forEach((type) => {
        const option = document.createElement('option');
        option.value = type;
        option.textContent = type;
        productTypeForMethodSelect.appendChild(option);
    });
}

function populateConnTypeSelect() {
    const connectTypeSelect = document.getElementById('connection-type-select');
    const ctPlaceholder = document.createElement('option');
    ctPlaceholder.value = '';
    ctPlaceholder.textContent = '-- Select connection type --';
    ctPlaceholder.disabled = true;
    ctPlaceholder.selected = true;
    connectTypeSelect.appendChild(ctPlaceholder);
    CONN_TYPES.forEach((type) => {
        const option = document.createElement('option');
        option.value = type;
        option.textContent = type;
        connectTypeSelect.appendChild(option);
    });
    connectTypeSelect.addEventListener('change', (e) => {
        renderConnConfig(e.target.value);
    });
}

// Populate dropdowns immediately from static data (no server needed)
populateProductTypeSelects();
populateConnTypeSelect();


function renderConfigFields(variant, container) {
    container.innerHTML = '';
    for (const [paramName, paramType] of Object.entries(variant)) {
        const inputType = paramType === 'int' ? 'number' : 'text';
        const div = document.createElement('div');
        div.innerHTML = `<label>${paramName}: <input type="${inputType}" id="conn_config_${paramName}" data-param-type="${paramType}"></label>`;
        container.appendChild(div);
    }
}

function renderConnConfig(connType) {
    const container = document.getElementById('conn_config');
    container.innerHTML = '';

    const variants = CONN_CONFIG_SCHEMA[connType];
    if (!variants || variants.length === 0) return;

    const fieldsDiv = document.createElement('div');
    fieldsDiv.id = 'conn-config-fields';

    if (variants.length > 1) {
        const variantSelect = document.createElement('select');
        variantSelect.id = 'conn-config-variant-select';
        variants.forEach((_, i) => {
            const opt = document.createElement('option');
            opt.value = i;
            opt.textContent = `Config ${i + 1}`;
            variantSelect.appendChild(opt);
        });
        variantSelect.addEventListener('change', () => {
            renderConfigFields(variants[variantSelect.value], fieldsDiv);
        });
        container.appendChild(variantSelect);
    }

    container.appendChild(fieldsDiv);
    renderConfigFields(variants[0], fieldsDiv);
}

function readConnConfig() {
    const config = {};
    const fieldsDiv = document.getElementById('conn-config-fields');
    if (!fieldsDiv) return config;
    const inputs = fieldsDiv.querySelectorAll('input');
    inputs.forEach(input => {
        const name = input.id.replace('conn_config_', '');
        const type = input.dataset.paramType;
        const raw = input.value.trim();
        if (raw === '') return;
        config[name] = type === 'int' ? parseInt(raw, 10) : raw;
    });
    return config;
}

const healthCheckBtn = document.getElementById('health-check-btn');
healthCheckBtn.addEventListener('click', () => {
    const healthCheckResult = document.getElementById('health-check-result');
    healthCheckResult.innerHTML = 'Checking health...';
    fetch(urlBuilder('/health'))
        .then((response) => {
            if (!response.ok) {
                throw new Error(`Health check failed: ${response.status}`);
            }
            return response.json();
        })
        .then((data) => {
            healthCheckResult.innerHTML = `Health check passed: ${JSON.stringify(data)}`;
        })
        .catch((error) => {
            healthCheckResult.innerHTML = `Health check failed: ${error.message}`;
        });
});

document.getElementById('hand-create-btn').addEventListener('click', () => {
    const handType = document.getElementById('product-type-select').value;
    const connectType = document.getElementById('connection-type-select').value;
    const connectConfig = readConnConfig();
    const createResult = document.getElementById('hand-create-result');
    createResult.innerHTML = 'Creating hand...';
    fetch(urlBuilder('/v1/hands'), {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            hand_type: handType,
            conn_method: connectType,
            conn_config: connectConfig,
        }),
    })
        .then((response) => {
            if (!response.ok) {
                throw new Error(`Hand creation failed: ${response.status}`);
            }
            return response.json();
        })
        .then((data) => {
            createResult.innerHTML = `Hand created: ${JSON.stringify(data)}`;
        })
        .catch((error) => {
            createResult.innerHTML = `Hand creation failed: ${error.message}`;
        });
});

document.getElementById('hand-query-btn').addEventListener('click', () => {
    const queryResult = document.getElementById('hand-query-result');
    queryResult.innerHTML = 'Querying hand...';
    fetch(urlBuilder('/v1/hands'))
        .then((response) => {
            if (!response.ok) {
                throw new Error(`Hand query failed: ${response.status}`);
            }
            return response.json();
        })
        .then((data) => {
            queryResult.innerHTML = `Hand query result: ${JSON.stringify(data)}`;
            const hands = Array.isArray(data) ? data : (data.hands || []);
            if (hands.length >= 0) {
                updateHandsCache(hands);
            }
        }).catch((error) => {
            queryResult.innerHTML = `Hand query failed: ${error.message}`;
        });
});

document.getElementById('hand-delete-btn').addEventListener('click', () => {
    const handId = document.getElementById('hand-id-input').value.trim();
    if (!handId) {
        document.getElementById('hand-delete-result').innerHTML = 'Please enter a hand ID.';
        return;
    }
    const deleteResult = document.getElementById('hand-delete-result');
    deleteResult.innerHTML = 'Deleting hand...';
    fetch(urlBuilder(`/v1/hands/${handId}`), {
        method: 'DELETE',
    })
        .then((response) => {
            if (!response.ok) {
                throw new Error(`Hand deletion failed: ${response.status}`);
            }
            return response.json();
        })
        .then((data) => {
            deleteResult.innerHTML = `Hand deleted: ${JSON.stringify(data)}`;
            updateHandsCache(handsCache.filter((hand) => String(hand.hand_id) !== String(handId)));
        })
        .catch((error) => {
            deleteResult.innerHTML = `Hand deletion failed: ${error.message}`;
        });
});

function populateMethodCallHandSelect(hands) {
    populateHandSelect('method-call-hand-select', hands, '-- Select hand ID --');
}

document.getElementById('method-call-hand-select').addEventListener('change', (e) => {
    const handId = e.target.value;
    const methodSel = document.getElementById('method-for-method-call-select');
    const paramContainer = document.getElementById('param-container');
    const descEl = document.getElementById('method-description');
    methodSel.innerHTML = '<option value="" disabled selected>-- Loading methods... --</option>';
    methodSel.disabled = true;
    paramContainer.innerHTML = '';
    descEl.textContent = '';

    if (methodsCache[handId]) {
        renderMethodOptions(methodsCache[handId]);
        return;
    }

    fetch(urlBuilder(`/v1/hands/${encodeURIComponent(handId)}/methods`))
        .then((r) => {
            if (!r.ok) throw new Error(`Methods fetch failed: ${r.status}`);
            return r.json();
        })
        .then((data) => {
            methodsCache[handId] = data.methods || [];
            renderMethodOptions(methodsCache[handId]);
        })
        .catch((err) => {
            methodSel.innerHTML = `<option value="" disabled selected>Failed: ${err.message}</option>`;
        });
});

function renderMethodOptions(methods) {
    const methodSel = document.getElementById('method-for-method-call-select');
    methodSel.innerHTML = '<option value="" disabled selected>-- Select method --</option>';
    methods.forEach((m) => {
        const opt = document.createElement('option');
        opt.value = m.name;
        opt.textContent = m.name;
        methodSel.appendChild(opt);
    });
    methodSel.disabled = false;
}

document.getElementById('method-for-method-call-select').addEventListener('change', (e) => {
    const handId = document.getElementById('method-call-hand-select').value;
    const methodName = e.target.value;
    const methods = methodsCache[handId] || [];
    const method = methods.find((m) => m.name === methodName);
    const paramContainer = document.getElementById('param-container');
    const descEl = document.getElementById('method-description');

    paramContainer.innerHTML = '';
    descEl.textContent = method ? method.description : '';

    if (!method) return;
    const paramNames = Array.isArray(method.params) ? method.params : Object.keys(method.params || {});
    if (paramNames.length === 0) return;

    // Build a template JSON object so user knows what keys to fill
    const template = {};
    paramNames.forEach(p => { template[p] = null; });
    const textarea = document.createElement('textarea');
    textarea.id = 'param-json-input';
    textarea.rows = 4;
    textarea.style.cssText = 'width:100%;font-family:monospace;font-size:13px;border:1px solid #d0d0d0;border-radius:6px;padding:8px;resize:vertical;';
    textarea.placeholder = 'Enter params as JSON';
    textarea.value = JSON.stringify(template, null, 2);
    const hint = document.createElement('div');
    hint.style.cssText = 'font-size:12px;color:#888;margin-top:4px;';
    hint.textContent = `Params: ${paramNames.join(', ')}`;
    paramContainer.appendChild(textarea);
    paramContainer.appendChild(hint);
});

document.getElementById('method-call-btn').addEventListener('click', () => {
    const handId = document.getElementById('method-call-hand-select').value;
    const methodName = document.getElementById('method-for-method-call-select').value;
    const result = document.getElementById('method-call-result');

    if (!handId) { result.innerHTML = 'Please select a hand.'; return; }
    if (!methodName) { result.innerHTML = 'Please select a method.'; return; }

    let body = {};
    const textarea = document.getElementById('param-json-input');
    if (textarea && textarea.value.trim()) {
        try {
            body = JSON.parse(textarea.value);
        } catch (e) {
            result.innerHTML = `Invalid JSON params: ${e.message}`;
            return;
        }
    }

    result.innerHTML = 'Calling...';
    fetch(urlBuilder(`/v1/hands/${encodeURIComponent(handId)}/methods?method=${encodeURIComponent(methodName)}`), {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body),
    })
        .then((r) => {
            if (!r.ok) throw new Error(`Call failed: ${r.status}`);
            return r.json();
        })
        .then((data) => {
            result.innerHTML = JSON.stringify(data, null, 2);
        })
        .catch((err) => {
            result.innerHTML = `Error: ${err.message}`;
        });
});



document.getElementById('method-query-type-select').addEventListener('change', (e) => {
    const productTypeSelect = document.getElementById('product-type-for-method-select');
    const handIdSelect = document.getElementById('hand-id-for-method-select');
    const queryType = e.target.value;

    if (queryType === 'product_type') {
        productTypeSelect.style.display = '';
        handIdSelect.style.display = 'none';
    } else if (queryType === 'hand_id') {
        productTypeSelect.style.display = 'none';
        handIdSelect.style.display = '';

        if (handsCache.length > 0) {
            populateMethodQueryHandSelect(handsCache);
            return;
        }
        fetch(urlBuilder('/v1/hands'))
            .then((response) => {
                if (!response.ok) throw new Error(`Hands fetch failed: ${response.status}`);
                return response.json();
            })
            .then((data) => {
                updateHandsCache(Array.isArray(data) ? data : (data.hands || []));
            })
            .catch((error) => {
                console.error('Failed to fetch hands:', error);
                handIdSelect.innerHTML = '<option value="" disabled selected>Failed to load hands</option>';
            });
    }
});

document.getElementById('hand-method-query-btn').addEventListener('click', () => {
    const queryType = document.getElementById('method-query-type-select').value;
    const queryResult = document.getElementById('hand-method-query-result');

    let queryValue;
    if (queryType === 'product_type') {
        queryValue = document.getElementById('product-type-for-method-select').value;
    } else if (queryType === 'hand_id') {
        queryValue = document.getElementById('hand-id-for-method-select').value;
    } else {
        queryResult.innerHTML = 'Please select a query type.';
        return;
    }

    if (!queryValue) {
        queryResult.innerHTML = 'Please select a value.';
        return;
    }

    queryResult.innerHTML = 'Querying hand methods...';
    let url;
    if (queryType === 'product_type') {
        url = urlBuilder(`/v1/hands/methods?product_type=${encodeURIComponent(queryValue)}`);
    } else {
        url = urlBuilder(`/v1/hands/${encodeURIComponent(queryValue)}/methods`);
    }
    fetch(url)
        .then((response) => {
            if (!response.ok) {
                throw new Error(`Hand method query failed: ${response.status}`);
            }
            return response.json();
        })
        .then((data) => {
            queryResult.innerHTML = `Hand method query result: ${JSON.stringify(data)}`;
        })
        .catch((error) => {
            queryResult.innerHTML = `Hand method query failed: ${error.message}`;
        });
});

function setWsStatus(text) {
    setText('ws-status', text);
}

function appendWsLog(label, payload) {
    const log = document.getElementById('ws-log');
    if (!log) return;
    const timestamp = new Date().toLocaleTimeString();
    const detail = typeof payload === 'string' ? payload : prettyJson(payload);
    const nextEntry = `[${timestamp}] ${label}\n${detail}`;
    const current = log.textContent ? `\n\n${log.textContent}` : '';
    log.textContent = `${nextEntry}${current}`.slice(0, 12000);
}

function updateWsControls() {
    const connecting = wsClient.socket?.readyState === WebSocket.CONNECTING;
    const connected = wsClient.connected && wsClient.socket?.readyState === WebSocket.OPEN;
    const connectBtn = document.getElementById('ws-connect-btn');
    const disconnectBtn = document.getElementById('ws-disconnect-btn');
    if (connectBtn) connectBtn.disabled = connected || connecting;
    if (disconnectBtn) disconnectBtn.disabled = !connected && !connecting;

    document.querySelectorAll('.ws-action').forEach((button) => {
        button.disabled = !connected;
    });
    const methodSelect = document.getElementById('ws-method-select');
    const paramsInput = document.getElementById('ws-param-json-input');
    document.querySelectorAll('.ws-hand-select').forEach((select) => {
        select.disabled = !connected;
    });
    if (methodSelect) methodSelect.disabled = !connected;
    if (paramsInput) paramsInput.disabled = !connected;
}

function rejectPendingWsRequests(error) {
    wsClient.pending.forEach((pending) => {
        clearTimeout(pending.timeoutId);
        pending.reject(error);
    });
    wsClient.pending.clear();
}

function initWsChart(stream) {
    if (wsCharts[stream]) {
        return wsCharts[stream];
    }

    const chartEl = document.getElementById(WS_CHART_IDS[stream]);
    if (!chartEl) {
        return null;
    }
    if (!window.echarts) {
        chartEl.textContent = 'ECharts CDN not loaded';
        chartEl.style.display = 'flex';
        chartEl.style.alignItems = 'center';
        chartEl.style.justifyContent = 'center';
        chartEl.style.color = '#888';
        chartEl.style.fontSize = '13px';
        return null;
    }

    chartEl.textContent = '';
    chartEl.style.display = '';
    chartEl.style.alignItems = '';
    chartEl.style.justifyContent = '';
    chartEl.style.color = '';
    chartEl.style.fontSize = '';
    wsCharts[stream] = window.echarts.init(chartEl);
    return wsCharts[stream];
}

function initWsCharts() {
    WS_STREAMS.forEach((stream) => {
        initWsChart(stream);
    });
}

function numericArray(value) {
    if (!Array.isArray(value)) {
        return null;
    }
    const numbers = value.filter((item) => typeof item === 'number' && Number.isFinite(item));
    return numbers.length > 0 ? numbers : null;
}

function categoryLabels(length, prefix = 'J') {
    return Array.from({ length }, (_, index) => `${prefix}${index}`);
}

function seriesFromNumericKeys(data, keys, type = 'line') {
    const series = [];
    let maxLength = 0;
    keys.forEach((key) => {
        const values = numericArray(data?.[key]);
        if (!values) {
            return;
        }
        maxLength = Math.max(maxLength, values.length);
        series.push({ name: key, type, data: values });
    });
    return { series, maxLength };
}

function stateChartOption(data) {
    const { series, maxLength } = seriesFromNumericKeys(
        data,
        ['joint_positions', 'joint_velocities', 'active_joint_angles', 'control_modes'],
        'line');
    return buildWsChartOption('State', categoryLabels(maxLength), series);
}

function diagnosticsChartOption(data) {
    const { series, maxLength } = seriesFromNumericKeys(
        data,
        ['temperature_reports', 'current_reports', 'current_thresholds'],
        'line');
    const errorValues = Array.isArray(data?.error_reports)
        ? data.error_reports
            .map((report) => report?.value)
            .filter((value) => typeof value === 'number' && Number.isFinite(value))
        : [];
    if (errorValues.length > 0) {
        series.push({ name: 'error_report_values', type: 'line', data: errorValues });
    }
    return buildWsChartOption(
        'Diagnostics',
        categoryLabels(Math.max(maxLength, errorValues.length)),
        series);
}

function tactileValues(sensor) {
    const direct = numericArray(sensor?.data);
    if (direct) {
        return direct;
    }
    return numericArray(sensor?.data?.channel_values);
}

function tactileChartOption(data) {
    const series = [];
    let maxLength = 0;
    const sensors = Array.isArray(data?.sensors) ? data.sensors : [];
    sensors.forEach((sensor, index) => {
        const values = tactileValues(sensor);
        if (!values) {
            return;
        }
        maxLength = Math.max(maxLength, values.length);
        const sensorName = sensor?.sensor_name ?? sensor?.sensor_id ?? `sensor_${index}`;
        series.push({ name: String(sensorName), type: 'line', data: values });
    });
    return buildWsChartOption('Tactile', categoryLabels(maxLength, 'C'), series);
}

function buildWsChartOption(title, labels, series) {
    if (series.length === 0) {
        return {
            title: { text: `${title}: no numeric data`, left: 'center', top: 'middle', textStyle: { fontSize: 13, color: '#888' } },
            xAxis: { show: false, type: 'category', data: [] },
            yAxis: { show: false, type: 'value' },
            series: [],
        };
    }

    return {
        tooltip: { trigger: 'axis' },
        legend: { type: 'scroll', top: 0, textStyle: { fontSize: 10 } },
        grid: { left: 42, right: 12, top: 48, bottom: 34 },
        xAxis: { type: 'category', data: labels },
        yAxis: { type: 'value', scale: true },
        series,
    };
}

function updateWsChart(stream, data) {
    const chart = initWsChart(stream);
    if (!chart) {
        return;
    }

    const optionBuilders = {
        state: stateChartOption,
        diagnostics: diagnosticsChartOption,
        tactile: tactileChartOption,
    };
    chart.setOption(optionBuilders[stream](data), true);
}

function handleWsEvent(message) {
    const stream = message.stream;
    if (!WS_STREAMS.includes(stream)) {
        appendWsLog('event', message);
        return;
    }

    const targetId = `ws-${stream}-result`;
    setText(targetId, prettyJson({
        hand_id: message.hand_id,
        data: message.data ?? null,
    }));
    updateWsChart(stream, message.data ?? {});
    appendWsLog(`event:${stream}`, {
        hand_id: message.hand_id,
        stream,
    });
}

function handleWsResult(message) {
    const requestKey = String(message.request_id ?? '');
    const pending = wsClient.pending.get(requestKey);
    appendWsLog(message.ok ? 'result' : 'error', message);

    if (!pending) {
        return;
    }

    clearTimeout(pending.timeoutId);
    wsClient.pending.delete(requestKey);
    if (message.ok) {
        pending.resolve(message.result);
    } else {
        pending.reject(new Error(message.error || 'WebSocket request failed'));
    }
}

function handleWsMessage(event) {
    let message;
    try {
        message = JSON.parse(event.data);
    } catch (error) {
        appendWsLog('invalid message', error.message);
        return;
    }

    if (message?.type === 'result') {
        handleWsResult(message);
        return;
    }
    if (message?.type === 'event') {
        handleWsEvent(message);
        return;
    }
    appendWsLog('message', message);
}

function connectWs() {
    if (wsClient.connected || wsClient.socket?.readyState === WebSocket.CONNECTING) {
        return;
    }

    const wsUrl = buildWsUrl();
    const socket = new WebSocket(wsUrl);
    wsClient.socket = socket;
    setWsStatus(`Connecting ${wsUrl}`);
    updateWsControls();

    socket.addEventListener('open', () => {
        wsClient.connected = true;
        setWsStatus(`Connected ${wsUrl}`);
        appendWsLog('connected', wsUrl);
        updateWsControls();
    });

    socket.addEventListener('message', handleWsMessage);

    socket.addEventListener('error', () => {
        appendWsLog('socket error', 'WebSocket connection error');
    });

    socket.addEventListener('close', () => {
        if (wsClient.socket === socket) {
            wsClient.socket = null;
        }
        wsClient.connected = false;
        rejectPendingWsRequests(new Error('WebSocket connection closed'));
        setWsStatus('Disconnected');
        appendWsLog('disconnected', wsUrl);
        updateWsControls();
    });
}

function disconnectWs() {
    const socket = wsClient.socket;
    if (!socket) {
        return;
    }
    if (socket.readyState === WebSocket.OPEN || socket.readyState === WebSocket.CONNECTING) {
        socket.close(1000, 'Client disconnect');
    }
}

function sendWsRequest(message) {
    const socket = wsClient.socket;
    if (!wsClient.connected || socket?.readyState !== WebSocket.OPEN) {
        return Promise.reject(new Error('WebSocket is not connected'));
    }

    wsClient.requestId += 1;
    const request = { ...message, request_id: wsClient.requestId };
    const requestKey = String(request.request_id);

    return new Promise((resolve, reject) => {
        const timeoutId = setTimeout(() => {
            wsClient.pending.delete(requestKey);
            reject(new Error(`WebSocket request timed out after ${WS_REQUEST_TIMEOUT_MS} ms`));
        }, WS_REQUEST_TIMEOUT_MS);

        wsClient.pending.set(requestKey, { resolve, reject, timeoutId });

        try {
            socket.send(JSON.stringify(request));
            appendWsLog('send', request);
        } catch (error) {
            clearTimeout(timeoutId);
            wsClient.pending.delete(requestKey);
            reject(error);
        }
    });
}

function wsSelectedHandId(selectId = 'ws-hand-select') {
    return document.getElementById(selectId)?.value ?? '';
}

function methodParamNames(method) {
    if (Array.isArray(method?.params)) {
        return method.params;
    }
    if (method?.params && typeof method.params === 'object') {
        return Object.keys(method.params);
    }
    return [];
}

function renderWsMethodOptions(methods) {
    const select = document.getElementById('ws-method-select');
    const paramsInput = document.getElementById('ws-param-json-input');
    if (!select) return;
    setSelectPlaceholder(select, '-- Select method --');
    methods.forEach((method) => {
        const option = document.createElement('option');
        option.value = method.name;
        option.textContent = method.name;
        select.appendChild(option);
    });
    if (paramsInput) {
        paramsInput.value = '{}';
    }
}

function setWsParamTemplate() {
    const handId = wsSelectedHandId('ws-method-hand-select');
    const methodName = document.getElementById('ws-method-select')?.value ?? '';
    const paramsInput = document.getElementById('ws-param-json-input');
    if (!paramsInput) return;

    const method = (wsClient.methodsCache[handId] ?? []).find((item) => item.name === methodName);
    const template = {};
    methodParamNames(method).forEach((paramName) => {
        template[paramName] = null;
    });
    paramsInput.value = prettyJson(template);
}

async function runWsAction(action) {
    try {
        await action();
    } catch (error) {
        appendWsLog('action failed', error.message);
    }
}

document.getElementById('ws-connect-btn')?.addEventListener('click', connectWs);
document.getElementById('ws-disconnect-btn')?.addEventListener('click', disconnectWs);

document.getElementById('ws-list-btn')?.addEventListener('click', () => {
    runWsAction(async () => {
        const result = await sendWsRequest({ type: 'list' });
        updateHandsCache(Array.isArray(result) ? result : []);
        setText('ws-hands-result', prettyJson(result));
    });
});

document.getElementById('ws-create-btn')?.addEventListener('click', () => {
    runWsAction(async () => {
        const productType = document.getElementById('product-type-select')?.value ?? '';
        const connMethod = document.getElementById('connection-type-select')?.value ?? '';
        if (!productType) {
            throw new Error('Please select a product type.');
        }
        if (!connMethod) {
            throw new Error('Please select a connection type.');
        }
        const result = await sendWsRequest({
            type: 'create',
            product_type: productType,
            conn_method: connMethod,
            conn_config: readConnConfig(),
            hand_side: 'left',
        });
        updateHandsCache([result, ...handsCache.filter((hand) => hand.hand_id !== result.hand_id)]);
        setText('ws-hands-result', prettyJson(result));
    });
});

document.getElementById('ws-remove-btn')?.addEventListener('click', () => {
    runWsAction(async () => {
        const handId = wsSelectedHandId();
        if (!handId) {
            throw new Error('Please select a hand.');
        }
        const result = await sendWsRequest({ type: 'remove', hand_id: Number(handId) });
        updateHandsCache(handsCache.filter((hand) => String(hand.hand_id) !== String(handId)));
        setText('ws-hands-result', prettyJson(result));
    });
});

document.getElementById('ws-load-methods-btn')?.addEventListener('click', () => {
    runWsAction(async () => {
        const handId = wsSelectedHandId('ws-method-hand-select');
        if (!handId) {
            throw new Error('Please select a hand.');
        }
        const result = await sendWsRequest({ type: 'methods', hand_id: Number(handId) });
        wsClient.methodsCache[handId] = result.methods ?? [];
        renderWsMethodOptions(wsClient.methodsCache[handId]);
        setText('ws-method-call-result', prettyJson(result));
    });
});

document.getElementById('ws-method-select')?.addEventListener('change', setWsParamTemplate);

document.getElementById('ws-method-hand-select')?.addEventListener('change', () => {
    renderWsMethodOptions([]);
});

document.getElementById('ws-call-method-btn')?.addEventListener('click', () => {
    runWsAction(async () => {
        const handId = wsSelectedHandId('ws-method-hand-select');
        const method = document.getElementById('ws-method-select')?.value ?? '';
        if (!handId) {
            throw new Error('Please select a hand.');
        }
        if (!method) {
            throw new Error('Please select a method.');
        }

        let params = {};
        const rawParams = document.getElementById('ws-param-json-input')?.value.trim() ?? '';
        if (rawParams) {
            params = JSON.parse(rawParams);
        }

        const result = await sendWsRequest({
            type: 'call',
            hand_id: Number(handId),
            method,
            params,
        });
        setText('ws-method-call-result', prettyJson(result));
    });
});

function runWsStreamAction(stream, type) {
    runWsAction(async () => {
        const handId = wsSelectedHandId(`ws-${stream}-hand-select`);
        if (!handId) {
            throw new Error('Please select a hand.');
        }
        const result = await sendWsRequest({
            type,
            hand_id: Number(handId),
            streams: [stream],
        });
        setText(`ws-${stream}-stream-result`, prettyJson(result));
    });
}

WS_STREAMS.forEach((stream) => {
    document.getElementById(`ws-${stream}-subscribe-btn`)?.addEventListener('click', () => {
        runWsStreamAction(stream, 'subscribe');
    });
    document.getElementById(`ws-${stream}-unsubscribe-btn`)?.addEventListener('click', () => {
        runWsStreamAction(stream, 'unsubscribe');
    });
});

updateWsControls();
initWsCharts();
window.addEventListener('resize', () => {
    Object.values(wsCharts).forEach((chart) => {
        chart?.resize();
    });
});
