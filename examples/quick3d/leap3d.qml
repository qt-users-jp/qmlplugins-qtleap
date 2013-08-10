import QtQuick 2.1
import Qt3D 2.0
import Qt3D.Shapes 2.0
import QtLeap 0.1

Viewport {
    id: root
    width: 600
    height: 480

    camera: Camera {
        id: camera
        eye: Qt.vector3d(0, 1000, 100)
        farPlane: 10000
    }

    light: Light {
        position: camera.eye
    }

    CylinderMesh {
        id: handMesh
        radius: 0.5
        length: 0.25
    }

    Effect {
        id: handEffect
        color: "#CCCCCC"
    }

    CylinderMesh {
        id: fingerMesh
        radius: 0.5
        length: 1
    }

    Effect {
        id: fingerEffect
        color: "#CCCCCC"
    }

    CylinderMesh {
        id: swipeMesh
        radius: 1
        length: 1
    }

    Effect {
        id: swipeEffect
        color: "#9999FF"
    }

    CylinderMesh {
        id: circleMesh
        radius: 1
        length: 1
    }

    Effect {
        id: circleEffect
        color: "#99FF99"
    }

    Item3D {
        Repeater {
            model: HandModel { id: hands; enabled: true }
            delegate: Item3D {
                position: model.palmPosition
                Text {
                    // this is used to update position :(
                    text: '(%1, %2, %3)'.arg(model.palmPosition.x).arg(model.palmPosition.y).arg(model.palmPosition.z)
                }

                effect: handEffect
                mesh: handMesh
                transform: [
                    Translation3D {
                        translate: Qt.vector3d(0, -0.1, 0)
                    },
                    Scale3D {
                        scale: Qt.vector3d(model.sphereRadius, model.sphereRadius, model.sphereRadius)
                    }
                    , LookAt { subject: handNormal }
                ]
                Item3D {
                    id: handNormal
                    position: model.palmNormal
                }
            }
        }

        Repeater {
            model: FingerModel { enabled: true }
            delegate: Item3D {
                position: model.tipPosition
                effect: fingerEffect
                mesh: fingerMesh
                transform: [
                    Translation3D {
                        translate: Qt.vector3d(0, 0, -0.5)
                    },
                    Scale3D {
                        scale: Qt.vector3d(model.width, model.width, model.length)
                    }
                    , LookAt { subject: fingerDirection }
                ]
                Item3D {
                    id: fingerDirection
                    position: model.direction
                }
            }
        }

        Repeater {
            id: gestures
            model: GestureModel {
                id: gestureModel
                enabled: true
                gestures: [GestureModel.SwipeGesture, GestureModel.CircleGesture]
            }

            delegate: Item3D {
                id: gesture

                Item3D {
                    id: gesture2
                }

                transform: [
                    Translation3D { id: gestureTranslation },
                    Scale3D { id: gestureScale },
                    LookAt { subject: gesture2 }
                ]

                states: [
                    State {
                        name: "swipe"
                        when: model.type === GestureModel.SwipeGesture
                        PropertyChanges {
                            target: gesture
                            position: model.position
                            mesh: swipeMesh
                            effect: swipeEffect
                        }
                        PropertyChanges {
                            target: gesture2
                            position: model.direction
                        }
                        PropertyChanges {
                            target: gestureTranslation
                            translate: Qt.vector3d(0, 0, -0.5)
                        }
                        PropertyChanges {
                            target: gestureScale
                            scale: Qt.vector3d(10, 10, model.speed / 10)
                        }
                    },
                    State {
                        name: 'circle'
                        when: model.type === GestureModel.CircleGesture
                        PropertyChanges {
                            target: gesture
                            position: model.center
                            mesh: circleMesh
                            effect: circleEffect
                        }
                        PropertyChanges {
                            target: gestureScale
                            scale: Qt.vector3d(model.radius, model.radius, model.progress)
                        }
                        PropertyChanges {
                            target: gesture2
                            position: model.normal
                        }
                    }

                ]
            }
        }
    }
}
