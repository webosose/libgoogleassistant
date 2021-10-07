# libgoogleassistant

## Summary

Google Assistant library for webOS OSE
Refer to the [Assistant SDK documentation](https://developers.google.com/assistant/sdk/) for more information.

## Setup instructions (very important)

### Step #1. Get credentials file. It must be an end-user's credentials.

* Create google account. Configure a developer project and account Settings. following [these instructions](https://developers.google.com/assistant/sdk/guides/service/python/embed/config-dev-project-and-account)
* Go to the [Actions Console](https://console.actions.google.com/) and register your device model. following [these instructions](https://developers.google.com/assistant/sdk/guides/service/python/embed/register-device)
```
Please ensure all of that instructions.
Create project, Enable api, Register device model, set activity control and download credentials json file etc.
```
* Download OAuth-2.0 Client ID credentials file and move it to ``/etc/googleAssistant/client_secret.json`` on your device via scp.
```bash
On your local pc
$ scp <downloaded json file> root@<your device ip>:/etc/googleAssistant/client_secret.json

On your device
$ cd /etc/googleAssistant
$ ./get_credentials.sh
```
Input the code from showed url then It will create the ``credentials.json`` file.


### Step #2. Register device id.

```bash
$ cd /etc/googleAssistant
$ vi ./device_id.json
```
Modify id(device instance string. eg. my_webos) and model_id(registered by step #1) fields in device_id.json

```bash
$ ./register_device_id.sh
```
It will register your device id


### Step #3. Change (or override) the device model and device id for aiservice.

Change a configuration file and reboot
```bash
$ vi /etc/systemd/system.conf.d/googleassistant.conf
-------------------------------------------------
...
DefaultEnvironment=GOOGLEAI_DEVICE_MODEL={registered by step #1}
DefaultEnvironment=GOOGLEAI_DEVICE_ID={registered by step #2}
-------------------------------------------------
$ reboot
```
Or, add following variables to the file to override the configuration.
```bash
$ vi /var/systemd/system/env/ai.env
-------------------------------------------------
GOOGLEAI_DEVICE_MODEL="{registered by step #1}"
GOOGLEAI_DEVICE_ID="{registered by step #2}"
-------------------------------------------------
```

### Step #4. Reboot device or restart service daemon

```bash
$ reboot
```
or
```bash
$ systemctl restart ai
```


### Step #5. Start ai by luna api
Refer ``README.md`` of com.webos.service.ai


## Options

Google assistant supports [custom device actions](https://developers.google.com/assistant/sdk/guides/service/python/extend/custom-actions)
Refer the ``/etc/googleAssistant/action.en.json``

You can download [gactions CLI tool](https://developers.google.com/actions/tools/gactions-cli)
