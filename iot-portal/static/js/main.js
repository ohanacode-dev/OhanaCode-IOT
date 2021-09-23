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

    }else if((device.type == 'thermometerc') || (device.type == 'thermometerf') || (device.type == 'clock')){
        var type_icon = "";
        var type_unit = "";
        if(device.type == 'thermometerc'){
            type_icon = "<i class='type-icon fas fa-thermometer-half'></i>";
            type_unit = "&#8451;";
        }else if(device.type == 'thermometerf'){
            type_icon = "<i class='type-icon fas fa-thermometer-half'></i>";
            type_unit = "&#8457;";
        }else if(device.type == 'clock'){
            type_icon = "<i class='type-icon far fa-clock'></i>";
        }

        for (let i = 0; i < device.current.length; i++) {
            new_dev_html += "<div class='text-display'>";
            new_dev_html += "<p class='text-data'>" + type_icon + "<span id='dev_" + device.id + "_" + i + "'>" + device.current[i] + "</span>" + type_unit + "</p></div>";
        }

    }else if((device.type == 'thermostatc') || (device.type == 'thermostatf') || (device.type == 'timer')){
        var type_icon = "";
        var type_unit = "";
        if(device.type == 'thermostatc'){
            type_icon = "<i class='type-icon fas fa-thermometer-half'></i>";
            type_unit = "&#8451;";
        }else if(device.type == 'thermostatf'){
            type_icon = "<i class='type-icon fas fa-thermometer-half'></i>";
            type_unit = "&#8457;";
        }else if(device.type == 'timer'){
            type_icon = "<i class='type-icon fas fa-stopwatch'></i>";
        }

        for (let i = 0; i < device.current.length; i++) {
            var target_icon = "<i class='type-icon fas fa-bullseye' id='ticon_" + device.id + "_" + i + "'></i>";

            new_dev_html += "<div class='text-display'>";
            new_dev_html += "<p class='text-data'>" + type_icon;
            new_dev_html += "<span id='dev_" + device.id + "_" + i + "'>" + device.current[i] + "</span>" + type_unit + "</p>";
            new_dev_html += "<p class='target_text'>" + target_icon;
            new_dev_html += "<span id='target_" + device.id + "_" + i + "'>" + device.target[i] + "</span>";
            new_dev_html += type_unit + "</p></div>";
        }
    }else{
        console.log("Unsupported device type:" + device.type);
    }

    var onclickhref = 'goto("http://' + device.addr + '")';
    new_dev_html += "<div class='break'></div>";
    new_dev_html += "<p class='setup' onclick='setLabel(\"" + device.id + "\");' title='Set label'><i class='fas fa-wrench'></i></p>";
    new_dev_html += "<p class='label'>" + device.label + "</p>";
    new_dev_html += "<p class='redirect' onclick='" + onclickhref + "' title='Open device UI'><i class='fas fa-arrow-circle-right'></i></p>";
    new_dev_html += "</div>";

    document.getElementById('device_list').innerHTML += new_dev_html;

    for (let i = 0; i < device.current.length; i++) {
        try{
            if (device.state[i] > 0){
                document.getElementById("ticon_" + device.id + "_" + i).style.color = 'red';
            }
        }catch (error){}
    }

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
                        document.getElementById("dev_" + device.id + "_" + i).value = device.current[i];
                    }
                }else if((device.type == 'thermometerc') || (device.type == 'thermometerf') || (device.type == 'clock')){
                    for (let i = 0; i < device.current.length; i++) {
                        document.getElementById("dev_" + device.id + "_" + i).innerHTML = device.current[i];
                    }
                }else if((device.type == 'thermostatc') || (device.type == 'thermostatf') || (device.type == 'timer')){
                    for (let i = 0; i < device.current.length; i++) {
                        document.getElementById("dev_" + device.id + "_" + i).innerHTML = device.current[i];

                        try{
                            document.getElementById("target_" + device.id + "_" + i).innerHTML = device.target[i];
                        }catch (error){}

                        try{
                            if (device.state[i] > 0){
                                document.getElementById("ticon_" + device.id + "_" + i).style.color = 'red';
                            }else{
                                document.getElementById("ticon_" + device.id + "_" + i).style.color = '#e7d7c3';
                            }
                        }catch (error){}
                    }
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

function goto(url){
    window.open(url, '_blank').focus();
}