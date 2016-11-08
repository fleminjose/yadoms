/**
 * Created by nicolasHILAIRE on 26/05/14.
 */

/**
 * Constructor which does nothing because it is used as a static class
 * @constructor
 */
function DeviceManager() { }

DeviceManager.factory = function (json) {
    assert(!isNullOrUndefined(json), "json must be defined");
    assert(!isNullOrUndefined(json.id), "json.id must be defined");
    assert(!isNullOrUndefined(json.pluginId), "json.pluginId must be defined");
    assert(!isNullOrUndefined(json.name), "json.name must be defined");
    assert(!isNullOrUndefined(json.friendlyName), "json.friendlyName must be defined");
    assert(!isNullOrUndefined(json.model), "json.model must be defined");

    return new Device(json.id, json.pluginId, json.name, json.friendlyName, json.model, json.configuration);
};

/**
 * Get a device from its id
 * @param {Integer|String} deviceId The device id to find
 * @return {Promise}
 */
DeviceManager.get = function (deviceId) {
    assert(!isNullOrUndefined(deviceId), "deviceId must be defined");

    var d = new $.Deferred();
    RestEngine.getJson("rest/device/" + deviceId)
    .done(function(data) {
        d.resolve(DeviceManager.factory(data));
    })
    .fail(d.reject);
    
    return d.promise();
};

/**
 * Get all devices
 * @ return {Promise}
 */
DeviceManager.getAll = function () {
    var d = new $.Deferred();

    RestEngine.getJson("rest/device")
    .done(function (data) {
        var devices = [];
        //foreach result we append a <tr>
        $.each(data.device, function (index, value) {
            devices.push(DeviceManager.factory(value));
        });
        d.resolve(devices);
    })
    .fail(d.reject);

    return d.promise();
};

/**
 * Get the device attached to a plugin
 * @param {Object} device The device
 * @ return {Promise}
 */
DeviceManager.getAttachedPlugin = function (device) {
    assert(!isNullOrUndefined(device), "device must be defined");

    var d = new $.Deferred();

    if(!device.attachedPlugin) {
        PluginInstanceManager.get(device.pluginId)
        .done(function (pluginInstance) {
            device.attachedPlugin = pluginInstance;
            d.resolve();
        }).fail(d.reject);
    } else {
       d.resolve();
    }

    return d.promise();
};

/**
 * Get the keywords attached to a device id
 * @param {Integer} deviceId The device id
 * @ return {Promise}
 */
DeviceManager.getKeywordsByDeviceId = function (deviceId) {
    assert(!isNullOrUndefined(deviceId), "deviceId must be defined");

    var d = new $.Deferred();
    RestEngine.getJson("/rest/device/" + deviceId + "/keyword")
    .done(function (data) {
        var list = [];
        $.each(data.keyword, function (index, value) {
            list.push(KeywordManager.factory(value));
        });
        d.resolve(list);
    })
    .fail(d.reject);

    return d.promise();
};

/**
 * Get the keywords attached to a device
 * @param {Object} device The device
 * @ return {Promise}
 */
DeviceManager.getKeywords = function (device) {
    assert(!isNullOrUndefined(device), "device must be defined");

    var d = new $.Deferred();

    if (isNullOrUndefined(device.keywords)) {
        DeviceManager.getKeywordsByDeviceId(device.id)
        .done(function (list) {
            device.keywords = list;
            d.resolve();
        })
        .fail(d.reject);
    }
    else {
        //keyword has already been gotten
        d.resolve();
    }

    return d.promise();
};

/**
 * Delete a device
 * @param {Object} device The device to delete
 * @param {Boolean} deleteDevice Indicate if the device is deleted
 * @ return {Promise}
 */
DeviceManager.deleteFromServer = function (device, deleteDevice) {
    assert(!isNullOrUndefined(device), "device must be defined");

    if (isNullOrUndefined(deleteDevice)) {
        deleteDevice = false;
    }

    return RestEngine.deleteJson("/rest/device/" + device.id + (deleteDevice ? "/removeDevice" : "/cleanupOnly"));
};

/**
 * Get a device configuration schema
 * @param {Object} device The device to get the configuration schema
 * @ return {Promise}
 */
DeviceManager.getConfigurationSchema = function(device) {

    var d = new $.Deferred();

    DeviceManager.getAttachedPlugin(device)   
    .done(function () {
        //get the plugin package.json
        PluginInstanceManager.downloadPackage(device.attachedPlugin)
        .done(function () {
            //try to get schema from the device model
            device.attachedPlugin.getPackageDeviceConfigurationSchema()
            .done(function(deviceConfig) {
                var schema = {};

                if(deviceConfig) {

                    //Manage static configuration
                    if(deviceConfig.staticConfigurationSchema) {

                        //find all static configurations matching the device model
                        var staticConfigMatchingDevice = _.filter(deviceConfig.staticConfigurationSchema, function(o) {
                            return _.some(o.models, function(model) {
                                return model == "*" || model == device.model;
                            });
                        });

                        //add it to resulting schema
                        _.forEach(staticConfigMatchingDevice, function(value) {
                            schema = _.merge(schema, value.schema);
                        });
                        
                    }

                    //Manage dynamic configuration
                    if(deviceConfig.dynamicConfigurationSchema && deviceConfig.dynamicConfigurationSchema == true) {

                        //ask the device configuration from the plugin instance 
                        RestEngine.getJson("/rest/device/" + device.id + "/configurationSchema")
                        .done(function (dynamicSchema) {
                            if(dynamicSchema)
                                schema = _.merge(schema, dynamicSchema);
                            d.resolve(schema);
                        }).fail(d.reject);                        
                    } else {
                        d.resolve(schema);
                    }
                } else {
                    //device configuration not exists in package.json
                    d.resolve(schema);
                }


            }).fail(d.reject);

        }).fail(d.reject);
    }).fail(d.reject);

    return d.promise();
}

/**
 * Update a device configuration
 * @param {Object} device The device to update
 * @ return {Promise}
 */
DeviceManager.updateToServer = function (device) {
    assert(!isNullOrUndefined(device), "device must be defined");

    var d = new $.Deferred();

    RestEngine.putJson("/rest/device/" + device.id + "/configuration", { data: JSON.stringify(device) })
    .done(function (data) {
        //it's okay
        //we update our information from the server
        device = DeviceManager.factory(data);
        d.resolve(device);
    }).fail(d.reject);

    return d.promise();
};