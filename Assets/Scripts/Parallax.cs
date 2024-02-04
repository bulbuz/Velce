using System.Collections;
using System.Collections.Generic;
using UnityEngine;  

public class Parallax : MonoBehaviour
{
    public Camera cam;

    // backgroundZ determines at what Z the parallax effect will be 100%, i.e. it follows the camera perfectly.
    private const float backgroundZ = -16f;

    private Vector2 origin;
    private float parallaxAmount;

    void Start()
    {
        origin = (Vector2)transform.position;
        // all objects are set to have whole number coordinates so they are synced up. If this is skipped and a
        // pixel-perfect camera is used, all parallax objects will move at differnet times which is ugly
        origin.x = Mathf.Round(origin.x);
        origin.y = Mathf.Round(origin.y);
        parallaxAmount = transform.position.z * (1 / backgroundZ);
    }

    // Update is called once per frame
    void Update()
    {
        Vector2 offset = new(cam.transform.position.x - origin.x, cam.transform.position.y - origin.y);
        offset *= parallaxAmount;
        transform.position = offset + origin;
    }
}
