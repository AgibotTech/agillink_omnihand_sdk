function parseTOML(text) {
    const config = {};
    let currentSection = config;
    const lines = text.split('\n');
    for (let i = 0; i < lines.length; i++) {
        const trimmed = lines[i].trim();
        if (!trimmed || trimmed.startsWith('#')) continue;
        const sectionMatch = trimmed.match(/^\[(.+)\]$/);
        if (sectionMatch) {
            currentSection = config[sectionMatch[1]] = {};
            continue;
        }
        const kvMatch = trimmed.match(/^(.+?)\s*=\s*\[$/);
        if (kvMatch) {
            const key = kvMatch[1].trim();
            const arr = [];
            i++;
            while (i < lines.length) {
                const arrLine = lines[i].trim();
                if (arrLine === ']') break;
                const elemMatch = arrLine.match(/^"(.+)",?$/);
                if (elemMatch) arr.push(elemMatch[1]);
                i++;
            }
            currentSection[key] = arr;
            continue;
        }
        const kvSingle = trimmed.match(/^(.+?)\s*=\s*(.+)$/);
        if (kvSingle) {
            let value = kvSingle[2].trim();
            if (value.startsWith('"') && value.endsWith('"')) {
                value = value.slice(1, -1);
            } else if (!isNaN(value)) {
                value = Number(value);
            }
            currentSection[kvSingle[1].trim()] = value;
        }
    }
    return config;
}
let configHost = null;
let connConfigData = {};
const urlBuilder = (path) => `http://${configHost.server.name}:${configHost.server.port}${path}`;


const configHostPath = 'configs/host.toml';

fetch(configHostPath)
    .then((response) => {
        if (!response.ok) {
            throw new Error(`Failed to load config: ${response.status}`);
        }
        return response.text();
    })
    .then((data) => {
        configHost = parseTOML(data);
        const serverHost = document.getElementById('server-host');
        serverHost.innerHTML = `${configHost.server.name}:${configHost.server.port}`;
        console.log('Config loaded:', configHost);

        return Promise.all([
            fetch(urlBuilder('/v1/hands/type?query=product_type'))
                .then((response) => {
                    if (!response.ok) throw new Error(`Product type query failed: ${response.status}`);
                    return response.json();
                })
                .then((data) => {
                    console.log('Product type query:', data);
                    const productTypeSelect = document.getElementById('product-type-select');
                    const ptPlaceholder = document.createElement('option');
                    ptPlaceholder.value = '';
                    ptPlaceholder.textContent = '-- Select product type --';
                    ptPlaceholder.disabled = true;
                    ptPlaceholder.selected = true;
                    productTypeSelect.appendChild(ptPlaceholder);
                    data.type.forEach((type) => {
                        const option = document.createElement('option');
                        option.value = type;
                        option.textContent = type;
                        productTypeSelect.appendChild(option);
                    });
                    const productTypeForMethodSelect = document.getElementById('product-type-for-method-select');
                    productTypeForMethodSelect.innerHTML = '';
                    productTypeForMethodSelect.appendChild(ptPlaceholder.cloneNode(true));
                    data.type.forEach((type) => {
                        const option = document.createElement('option');
                        option.value = type;
                        option.textContent = type;
                        productTypeForMethodSelect.appendChild(option);
                    });
                }),
            fetch(urlBuilder('/v1/hands/type?query=conn_type'))
                .then((response) => {
                    if (!response.ok) throw new Error(`Connection type query failed: ${response.status}`);
                    return response.json();
                })
                .then((data) => {
                    console.log('Connection type query:', data);
                    const connectTypeSelect = document.getElementById('connection-type-select');
                    const ctPlaceholder = document.createElement('option');
                    ctPlaceholder.value = '';
                    ctPlaceholder.textContent = '-- Select connection type --';
                    ctPlaceholder.disabled = true;
                    ctPlaceholder.selected = true;
                    connectTypeSelect.appendChild(ctPlaceholder);
                    data.type.forEach((type) => {
                        const option = document.createElement('option');
                        option.value = type;
                        option.textContent = type;
                        connectTypeSelect.appendChild(option);
                    });
                    connectTypeSelect.addEventListener('change', (e) => {
                        renderConnConfig(e.target.value);
                    });
                }),
            fetch(urlBuilder('/v1/hands/type?query=conn_config'))
                .then((response) => {
                    if (!response.ok) throw new Error(`Connection config query failed: ${response.status}`);
                    return response.json();
                })
                .then((data) => {
                    console.log('Connection config query:', data);
                    connConfigData = data.type || {};
                    const connectTypeSelect = document.getElementById('connection-type-select');
                    if (connectTypeSelect.value) {
                        renderConnConfig(connectTypeSelect.value);
                    }
                }),
        ]);
    })
    .catch((error) => {
        console.error('API initialization failed:', error);
        const serverHost = document.getElementById('server-host');
        if (!configHost) {
            serverHost.innerHTML = 'Failed to load config';
        }
    });


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

    const variants = connConfigData[connType];
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
            if (Array.isArray(data.hands)) {
                handsCache = data.hands;
                populateMethodCallHandSelect(data.hands);
            }
        }).catch((error) => {
            queryResult.innerHTML = `Hand query failed: ${error.message}`;
        });
});

document.getElementById('hand-vendor-info-btn').addEventListener('click', () => {
    const vendorInfoResult = document.getElementById('hand-vendor-info-result');
    vendorInfoResult.innerHTML = 'Querying vendor info...';
    fetch(urlBuilder(`/v1/hands/vendor-info`))
        .then((response) => {
            if (!response.ok) {
                throw new Error(`Vendor info query failed: ${response.status}`);
            }
            return response.json();
        })
        .then((data) => {
            vendorInfoResult.innerHTML = `Vendor info query result: ${JSON.stringify(data)}`;
        })
        .catch((error) => {
            vendorInfoResult.innerHTML = `Vendor info query failed: ${error.message}`;
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

    fetch(urlBuilder(`/v1/hands/methods?query=hand_id&q=${encodeURIComponent(handId)}`))
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

    if (!method || Object.keys(method.params).length === 0) return;

    for (const [paramName, paramType] of Object.entries(method.params)) {
        const isArray = paramType.endsWith('[]');
        const baseType = isArray ? paramType.slice(0, -2) : paramType;
        const inputType = baseType === 'integer' ? 'number' : 'text';
        const placeholder = isArray ? `comma-separated ${baseType} values` : paramType;
        const label = document.createElement('label');
        label.innerHTML = `${paramName} <span style="color:#aaa;font-size:11px">(${paramType})</span>
            <input type="${inputType}" id="param_${paramName}" placeholder="${placeholder}" data-type="${paramType}">`;
        paramContainer.appendChild(label);
    }
});

document.getElementById('method-call-btn').addEventListener('click', () => {
    const handId = document.getElementById('method-call-hand-select').value;
    const methodName = document.getElementById('method-for-method-call-select').value;
    const result = document.getElementById('method-call-result');

    if (!handId) { result.innerHTML = 'Please select a hand.'; return; }
    if (!methodName) { result.innerHTML = 'Please select a method.'; return; }

    const methods = methodsCache[handId] || [];
    const method = methods.find((m) => m.name === methodName);
    const body = {};

    if (method) {
        for (const [paramName, paramType] of Object.entries(method.params)) {
            const input = document.getElementById(`param_${paramName}`);
            if (!input) continue;
            const raw = input.value.trim();
            if (raw === '') continue;
            const isArray = paramType.endsWith('[]');
            const baseType = isArray ? paramType.slice(0, -2) : paramType;
            if (isArray) {
                body[paramName] = raw.split(',').map((v) => {
                    const t = v.trim();
                    return baseType === 'integer' ? parseInt(t, 10) : baseType === 'number' ? parseFloat(t) : t;
                });
            } else if (baseType === 'integer') {
                body[paramName] = parseInt(raw, 10);
            } else if (baseType === 'number') {
                body[paramName] = parseFloat(raw);
            } else if (baseType === 'boolean') {
                body[paramName] = raw === 'true';
            } else {
                body[paramName] = raw;
            }
        }
    }

    result.innerHTML = 'Calling...';
    fetch(urlBuilder(`/v1/hands/methods?query=hand_id&q=${encodeURIComponent(handId)}&method=${encodeURIComponent(methodName)}`), {
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
                handsCache = data.hands || [];
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
    fetch(urlBuilder(`/v1/hands/methods?query=${queryType}&q=${encodeURIComponent(queryValue)}`))
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