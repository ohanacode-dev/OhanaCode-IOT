var dev_ws = new WebSocket("ws://" + window.location.host + "/devices");
var dev_value_list = [];

function addNewDevice(device){
    var new_dev_html = "<div class='row device_div' id='dev_" + device.id + "'>";

    if(device.type == 'switch'){

        for (let i = 0; i < device.current.length; i++) {

            new_dev_html += "<div id='dev_" + device.id + "_" + i + "' class=";
            if(device.current[i] == 0){
                new_dev_html += "'lightBtnOff'";
            }else{
                new_dev_html += "'lightBtnOn'";
            }
            new_dev_html += " onclick='toggle(" + device.id + ", " + i + ");'>";
            new_dev_html += "<span></span></div>";
        }

    }else if(device.type == 'dimmer'){

        for (let i = 0; i < device.current.length; i++) {
            new_dev_html += "<div class='v-slider-wrapper'>" +
                            "<input type='range' id='dev_" + device.id + "_" + i + "' class='v-slider color_" + i + "' min='0' max='100' value='" +
                            device.current[i] + "' onchange='update_dimmer(" + device.id + ", " + i + ", this.value)'></div>";
        }

    }else if(device.type == 'thermometer'){

        for (let i = 0; i < device.current.length; i++) {
            new_dev_html += "<div class='thermometer'>";
            new_dev_html += "<p class='units'>&#8451;</p>";
            new_dev_html += "<p class= 'thermo-text' id='dev_" + device.id + "_" + i + "'>" + device.current[i] + "</p>"
            new_dev_html += "</div>";
        }

    }else if(device.type == 'thermostat'){
//                    new_dev_html = "<div class='thermostat_div' id='" + device.mac + "'><p class='temp'></p><p class='target'></p>";
    }else{
        console.log("Unsupported device type:" + device.type);
    }

    new_dev_html += "<div class='break'></div>";
    new_dev_html += "<p class='setup' onclick='setLabel(\"" + device.id + "\");' title='Set label'><i class='fas fa-wrench'></i></p>";
    new_dev_html += "<p class='label'>" + device.label + "</p>";
    new_dev_html += "<a class='redirect' href='http://" + device.addr + "' target='_blank' ' title='Open device UI'><i class='fas fa-arrow-circle-right'></i></a>";
    new_dev_html += "</div>";

    document.getElementById('device_list').innerHTML += new_dev_html;

    dev_value_list[device.id] = device.current;
}

dev_ws.onmessage = function(event) {
    try{
        response = JSON.parse(event.data)
        if(response.topic == 'device_list'){
            document.getElementById('device_list').innerHTML = "";

            new_global_html = "";
            for (const device_data of response.data) {
                device = JSON.parse(device_data.data);
                device['id'] = device_data.id;
                addNewDevice(device);
            }

        }else if(response.topic == 'set_device'){
            data = response.data;
            var device = JSON.parse(data.data);
            device['id'] = data.id;

            var html_device = document.getElementById("dev_" + device.id);
            if(typeof(html_device) == 'undefined' || html_device == null){
                addNewDevice(device);
            }else{
//                console.log(data);
                if(device.type == 'switch'){
                    for (let i = 0; i < device.current.length; i++) {
                        var switch_obj = document.getElementById("dev_" + device.id + "_" + i);
                        switch_obj.classList.remove("lightBtnOff");
                        switch_obj.classList.remove("lightBtnOn");
                        if(device.current[i] == 0){
                            switch_obj.classList.add("lightBtnOff");
                        }else{
                            switch_obj.classList.add("lightBtnOn");
                        }
                    }
                }else if(device.type == 'dimmer'){
                    for (let i = 0; i < device.current.length; i++) {
                        var dimmer_obj = document.getElementById("dev_" + device.id + "_" + i);
                        dimmer_obj.value = device.current[i];
                    }
                }else if(device.type == 'thermometer'){
                    for (let i = 0; i < device.current.length; i++) {
                        var dimmer_obj = document.getElementById("dev_" + device.id + "_" + i);
                        dimmer_obj.innerHTML = device.current[i];
                    }
                }else if(device.type == 'thermostat'){

                }else{
                    console.log("Unsupported device type:" + device.type);
                }

                dev_value_list[device.id] = device.current;
            }
        }
    }catch(err){
        console.log("Error parsing response:" + err);
    }
};

dev_ws.onopen = function(event){
    get_device_list();
}

function get_device_list() {
    var msg = { topic: "device_list" };
    dev_ws.send(JSON.stringify(msg));
}

function setLabel(device_id){
    var label = window.prompt("New label: ");
    if(label != null){
        var msg = { topic: "set_device", id: device_id, data: {"label": label}};
        dev_ws.send(JSON.stringify(msg));
    }
}

function toggle(dev_id, value_id){
    var dev_values = [...dev_value_list[dev_id]];
    var value = dev_values[value_id];
    if(value == 0){
        value = 1;
    }else{
        value = 0;
    }
    dev_values[value_id] = value;
    var msg = { topic: "set_device", id: dev_id, data: {"current": dev_values}};
    dev_ws.send(JSON.stringify(msg));
}

function update_dimmer(dev_id, value_id, value){
    var dev_values = [...dev_value_list[dev_id]];
    dev_values[value_id] = value;
    var msg = { topic: "set_device", id: dev_id, data: {"current": dev_values}};
    dev_ws.send(JSON.stringify(msg));
}