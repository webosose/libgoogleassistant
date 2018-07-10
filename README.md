libgoogleassistant
==================

Summary
-------
Google Assistant library for webOS OSE
Refer to the [Assistant SDK documentation](https://developers.google.com/assistant/sdk/) for more information.

Setup instructions (very important)
-----------------------------------
1. Get credentials file. It must be an end-user's credentials.
 * Create google account.
 * Go to the [Actions Console](https://console.actions.google.com/) and register your device model, following [these instructions](https://developers.google.com/assistant/sdk/guides/service/python/embed/register-device)
   ( Please ensure all of that instructions. create project, register device model, enable api, set activity control download credentials json file etc.
     you can use `register_device_model.sh` instead of web browser ui if you already have project. refer device_model.json template. )
 * Download OAuth-2.0 Client ID credentials file and move it to /etc/googleAssistant/client_secret.json on your device via scp.
 * Move /etc/googleAssistant folder
 * Run `get_credentials.sh`. It will create the file `credentials.json`.


3. Run `register_device_id.sh`. It will register device instance(unique) id using the rest api. refer device_id.json template.


4. Fill the device model and device id in /var/systemd/system/env/ai.env


5. Reboot device or restart service daemon.
 * $ reboot
   or
 * $ systemctl restart ai


6. Start ai by luna api.
 * Refer [README.md] of com.webos.service.ai


Options
-------
Google assistant supports [custom device actions](https://developers.google.com/assistant/sdk/guides/service/python/extend/custom-actions)
Refer the /etc/googleAssistant/action.en.json

You can download [gactions CLI tool](https://developers.google.com/actions/tools/gactions-cli)
