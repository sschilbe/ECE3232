using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class SampleController : MonoBehaviour
{
    public Button   button;
    public Text     nameText;
    public Image   connectedImage;

    private Controller controller;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    public void Setup( Controller currentController ) {
        controller = currentController;
        nameText.text = currentController.port.PortName;
        
        updateImage();
    }

    public void Connection() {
        controller.connected = !controller.connected;
        
        if( controller.connected ) {
            controller.kill = false;
            
           controller.OpenSerialPort();

            controller.sendMessage( controller.GetGameMessage( MsgId.CONNECT_EVENT, false ) );
            
            int timeout = 5000;

            while( !controller.connect_event ) {
                System.Threading.Thread.Sleep( 50 );

                timeout -= 50;
                if( timeout <= 0 ) {
                    controller.connected = !controller.connected;
                    break;
                }
            }

            controller.connect_event = false;

            if( controller.connected && MenuController.controller == null ) {
                MenuController.controller = controller;
            }

        } else {
            controller.sendMessage( controller.GetGameMessage( MsgId.DISCONNECT_EVENT, false ) );
            controller.assigned = false;
            controller.kill = true;
            controller.readThread.Join();
            controller.port.Close();
            MenuController.controller = null;
        }

        updateImage();

        GameObject button = GameObject.Find( "BackButton" );
        if( button != null ) {
            EventSystem.current.SetSelectedGameObject( button );
        }
    }

    private void updateImage() {
        if( controller.connected ) {
            button.GetComponentInChildren<Text>().text = "Disconnect";
            connectedImage.sprite = Resources.Load<Sprite>("green");
        } else {
            connectedImage.sprite = Resources.Load<Sprite>("red");
            button.GetComponentInChildren<Text>().text = "Connect";
        }
    }
}
