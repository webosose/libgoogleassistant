#!/bin/bash

#
# Copyright (c) 2018 LG Electronics, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0
#

# The input of this file is device_id.json file. Please fill your device instance information into that file.
# If succeeded, it will output credentials.json, which can be used with run_assistant.

PROJECT_ID=`cat credentials.json | grep -o -e '"project_id": "[^"]*"' | sed -e 's/.*"project_id": "\([^"]*\)".*/\1/g'`
ACCESS_TOKEN=`cat credentials.json | grep -o -e '"access_token": "[^"]*"' | sed -e 's/.*"access_token": "\([^"]*\)".*/\1/g'`
TOKEN_TYPE=`cat credentials.json | grep -o -e '"token_type": "[^"]*"' | sed -e 's/.*"token_type": "\([^"]*\)".*/\1/g'`

echo "project id  :" $PROJECT_ID
echo "access token:" $ACCESS_TOKEN
echo "token type  :" $TOKEN_TYPE

curl -s -k -X POST -H "Content-Type: application/json" \
    -H "Authorization: $TOKEN_TYPE $ACCESS_TOKEN" -d @device_id.json \
    https://embeddedassistant.googleapis.com/v1alpha2/projects/$PROJECT_ID/devices/
