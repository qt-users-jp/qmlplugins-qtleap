/* Copyright (c) 2012 Silk Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Silk nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SILK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick 2.1
import QtLeap 0.1

Rectangle {
    id: root
    width: 360
    height: 360
    color: 'darkBlue'

    PointableModel { id: pointables; enabled: true }
    FingerModel { id: fingers }
    ToolModel { id: tools }

    Text {
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        color: 'yellow'
        text: qsTr('Pointables')
    }

    Item {
        anchors.centerIn: parent
        Repeater {
            id: repeater
            width: 0
            height: 0
            model: pointables

            delegate: Rectangle {
                x: model.tipPosition.x
                y: model.tipPosition.z
                width: 2000 / model.tipPosition.y
                height: width
                radius: width / 2
                color: 'yellow'
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.state = 'fingers'
    }

    states: [
        State {
            name: 'fingers'
            PropertyChanges {
                target: mouseArea
                onClicked: root.state = 'tools'
            }
            PropertyChanges {
                target: label
                text: qsTr('Fingers')
            }
            PropertyChanges {
                target: repeater
                model: fingers
            }
            PropertyChanges {
                target: fingers
                enabled: true
            }
            PropertyChanges {
                target: pointables
                enabled: false
            }
        },
        State {
            name: 'tools'
            PropertyChanges {
                target: mouseArea
                onClicked: root.state = ''
            }
            PropertyChanges {
                target: label
                text: qsTr('Tools')
            }
            PropertyChanges {
                target: repeater
                model: tools
            }
            PropertyChanges {
                target: tools
                enabled: true
            }
            PropertyChanges {
                target: pointables
                enabled: false
            }
        }
    ]
}
