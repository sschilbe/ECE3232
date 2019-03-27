using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/**
 *
 * @file
 * ButtonSelector.cs
 *
 * @brief
 * Delay for board input control
 *
 * Author: Meneley, Julia | Schilbe, Seth
 * Date Created: 26/03/2019
 * Last Modified: 27/03/2019
 */
public class ButtonSelector : MonoBehaviour
{
private float delay = 0.5f;
    private bool changed;
    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        if( delay <= 0.0f ) {
            changed = MenuController.changeSelection();
            delay = 0.5f;
        } else {
            delay -= Time.deltaTime;
        }
    }
}
