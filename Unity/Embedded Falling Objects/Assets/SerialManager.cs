using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class SerialManager : MonoBehaviour {

	// Use this for initialization
	void Start() {
		foreach( string str in SerialPort.GetPortNames()) {
			Debug.Log( string.Format( "Port : {0}", str ) );
		}	
	}
	
	// Update is called once per frame
	void Update() {
		
	}

	public static void Read( Controller controller ) {
        while ( !controller.kill ) {
            try {
                string message = controller.port.ReadLine();
                string[] tokens = message.Split('|');
				int msg_id = 5;
				if( tokens.Length == 5 ) {
					try {
						msg_id = Convert.ToInt32( tokens[0] );
					} catch ( FormatException ) {

					}

					switch( msg_id ) {
						case (int)MsgId.CONNECT_EVENT:
							controller.connect_event = true;
							break;
						case (int)MsgId.DISCONNECT_EVENT:
							break;
						case (int)MsgId.PLAYER_HIT_EVENT:
							break;
						case (int)MsgId.PLAYER_DATA_EVENT:
							controller.xSpeed = float.Parse( tokens[2] ) / 1000;
							controller.ySpeed = float.Parse( tokens[3] ) / 1000;
							controller.zSpeed = float.Parse( tokens[4] ) / 1000;
							break;
					}
					controller.port.DiscardInBuffer();
				}
            } catch ( TimeoutException ) {
				// No-Op - catch timeouts but they don't matter
			}

			System.Threading.Thread.Sleep(10);
        }
	}
}
