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
let configHost = DEFAULT_API_BASE_URL;

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

loadRuntimeConfig();


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
                handsCache = hands;
                populateMethodCallHandSelect(hands);
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
        })
        .catch((error) => {
            deleteResult.innerHTML = `Hand deletion failed: ${error.message}`;
        });
});

let methodsCache = {};

function populateMethodCallHandSelect(hands) {
    const sel = document.getElementById('method-call-hand-select');
    sel.innerHTML = '<option value="" disabled selected>-- Select hand ID --</option>';
    hands.forEach((hand) => {
        const opt = document.createElement('option');
        opt.value = hand.hand_id;
        opt.textContent = `${hand.hand_id} (${hand.hand_type})`;
        sel.appendChild(opt);
    });
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

        if (handsCache) {
            handIdSelect.innerHTML = '';
            const ph = document.createElement('option');
            ph.value = ''; ph.textContent = '-- Select a hand --'; ph.disabled = true; ph.selected = true;
            handIdSelect.appendChild(ph);
            handsCache.forEach((hand) => {
                const opt = document.createElement('option');
                opt.value = hand.hand_id;
                opt.textContent = `${hand.hand_id} (${hand.hand_type})`;
                handIdSelect.appendChild(opt);
            });
            return;
        }
        fetch(urlBuilder('/v1/hands'))
            .then((response) => {
                if (!response.ok) throw new Error(`Hands fetch failed: ${response.status}`);
                return response.json();
            })
            .then((data) => {
                handsCache = Array.isArray(data) ? data : (data.hands || []);
                handIdSelect.innerHTML = '';
                const placeholder = document.createElement('option');
                placeholder.value = '';
                placeholder.textContent = '-- Select a hand --';
                placeholder.disabled = true;
                placeholder.selected = true;
                handIdSelect.appendChild(placeholder);
                handsCache.forEach((hand) => {
                    const option = document.createElement('option');
                    option.value = hand.hand_id;
                    option.textContent = `${hand.hand_id} (${hand.hand_type})`;
                    handIdSelect.appendChild(option);
                });
                populateMethodCallHandSelect(handsCache);
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
