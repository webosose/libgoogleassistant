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

PROJECT_ID=`cat credentials.json | grep -o -e '"project_id": "[^"]*"' | sed -e 's/.*"project_id": "\([^"]*\)".*/\1/g'`
DEVICE_ID=`cat device_id.json | grep -o -e '"id": "[^"]*"' | sed -e 's/.*"id": "\([^"]*\)".*/\1/g'`
ACCESS_TOKEN=`cat credentials.json | grep -o -e '"access_token": "[^"]*"' | sed -e 's/.*"access_token": "\([^"]*\)".*/\1/g'`
TOKEN_TYPE=`cat credentials.json | grep -o -e '"token_type": "[^"]*"' | sed -e 's/.*"token_type": "\([^"]*\)".*/\1/g'`

echo "project id     :" $PROJECT_ID
echo "device id      :" $DEVICE_ID
echo "access token   :" $ACCESS_TOKEN
echo "token type     :" $TOKEN_TYPE

curl -s -k -X DELETE -H "Content-Type: application/json" \
    -H "Authorization: $TOKEN_TYPE $ACCESS_TOKEN" \
    https://embeddedassistant.googleapis.com/v1alpha2/projects/$PROJECT_ID/devices/$DEVICE_ID
