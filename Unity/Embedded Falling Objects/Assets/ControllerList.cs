using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;
using System.IO.Ports;

/**
 *
 * @file
 * ControllerList.cs
 *
 * @brief
 * Manages a list of controllers and there associated states
 *
 * Author: Meneley, Julia | Schilbe, Seth
 * Date Created: 19/02/2019
 * Last Modified: 27/03/2019
 */
public enum MsgId { 
    CONNECT_EVENT, 	// Device has been connected to the game
	DISCONNECT_EVENT,		// Device has been disconnected from the game
	GAME_START_EVENT,		// The game has started
	PLAYER_HIT_EVENT,		// Player has been hit in the game
	PLAYER_DATA_EVENT,		// Player data sent to the game
	INVALID_EVENT			// Invalid event
};

[System.Serializable]
public class Controller {
	public SerialPort	port;
	public String		portName;
	public int			id;
	public int 			lives;
	public Thread		readThread;
	public bool			kill;
	public float xSpeed = 0;
	public float ySpeed = 0;
	public float zSpeed = 0;
	public Queue<string> txQueue = new Queue<string>();
	public bool			connected;
	public bool connect_event = false;
	public bool			assigned;
	public int defaultBaudRate = 9600;
	public int defaultTimeout = 2000;

	
	public void PortSetup( SerialPort port, string portName ) {
		port.PortName = portName;
		port.BaudRate = defaultBaudRate;
		port.ReadTimeout = 1;
		port.WriteTimeout = defaultTimeout;
		port.Parity = Parity.None;
		port.StopBits = StopBits.One;
		port.NewLine = "\n";
	}
	
    public string GetGameMessage( MsgId msgID, bool hit ) {
        string test =   Convert.ChangeType( msgID, msgID.GetTypeCode() ) + "|" 
                        + id + "|" 
                        + Convert.ToInt32( connected ) + "|" 
                        + Convert.ToInt32( hit ) + "|"
                        + lives;
        return test;
    }

	public void OpenSerialPort() {
		if( port == null ) {
			port = new SerialPort();
			PortSetup( port, portName );
		}

		// Check if the port is already open
        if( !port.IsOpen ) {
			port = new SerialPort();
			PortSetup( port, portName );
            port.Open();
        }

		port.DiscardInBuffer();
		
		if( readThread == null || ( readThread != null && !readThread.IsAlive ) ) {
			readThread = new Thread( ()=>SerialManager.Read( this ) );
			readThread.Start();
		}

    	port.DiscardInBuffer();
	}

	public void sendMessage( String message ) {
		if( port == null || ( (port != null) && !port.IsOpen ) ) {
			OpenSerialPort();

		}
		
		port.DiscardOutBuffer();
		port.WriteLine( message );
	}
}

public class ControllerList : MonoBehaviour {

	public static List<Controller> controllerList = new List<Controller>();
	public Transform		contentPanel;
	public SimpleObjectPool	controllerObjectPool;
	public int defaultStartingLives = 3;
	void Start() {
		RefreshDisplay();
	}

	public void RefreshDisplay() {
		RemoveControllers();
		FetchAllDevices();
		AddControllers();
	}

	private void FetchAllDevices() {
		for( int i = 0; i < SerialPort.GetPortNames().Length; i++ ) {
			// Convoluted function to check if the controller is already in the list
			if( !( controllerList.FindAll( c => c.port.PortName == SerialPort.GetPortNames()[i] ).Count > 0 ) ) {
				Controller controller = new Controller();
				controller.portName = SerialPort.GetPortNames()[i];
				controller.OpenSerialPort();

				controller.id = i;
				controller.connected = false;
				controller.kill = false;
				controller.lives = defaultStartingLives;
				controllerList.Add( controller );			
			}
		}
	}
	private void AddControllers() {
		for( int i = 0; i < controllerList.Count; i++ ) {
			Controller controller = controllerList[i];
			GameObject newController = controllerObjectPool.GetObject();
			newController.transform.SetParent( contentPanel );

			SampleController sampleController = newController.GetComponent<SampleController>();
			sampleController.Setup( controller );
		}
	}

	private void RemoveControllers() {
		while (contentPanel.childCount > 0) {
            GameObject toRemove = transform.GetChild(0).gameObject;
            controllerObjectPool.ReturnObject(toRemove);
        }
	}
}
