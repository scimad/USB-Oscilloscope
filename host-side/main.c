/* Name: hidtool.c
 * Project: hid-data example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-11
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hiddata.h"
#include "../firmware/usbconfig.h"  /* for device VID, PID, vendor name and product name */

/* ------------------------------------------------------------------------- */


#include <GL/glut.h>

int windowWidth, windowHeight;      //Global variable for graphics window
usbDevice_t *dev;                   //Global variable for usbDevice

/*--------------------------------------------------------------------------*/


/* ------------------------------------------------------------------------- */

static void hexdump(char *buffer, int len)
{
int     i;
FILE    *fp = stdout;

    for(i = 0; i < len; i++){
        if(i != 0){
            if(i % 16 == 0){
                fprintf(fp, "\n");
            }else{
                fprintf(fp, " ");
            }
        }
        fprintf(fp, "0x%02x", buffer[i] & 0xff);
    }
    if(i != 0)
        fprintf(fp, "\n");
}

static char *usbErrorMessage(int errCode)
{
static char buffer[80];

    switch(errCode){
        case USBOPEN_ERR_ACCESS:      return "Access to device denied";
        case USBOPEN_ERR_NOTFOUND:    return "The specified device was not found";
        case USBOPEN_ERR_IO:          return "Communication error with device";
        default:
            sprintf(buffer, "Unknown USB error %d", errCode);
            return buffer;
    }
    return NULL;    /* not reached */
}

static usbDevice_t  *openDevice(void)
{
    usbDevice_t     *dev = NULL;
    unsigned char   rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
    char            vendorName[] = {USB_CFG_VENDOR_NAME, 0}, productName[] = {USB_CFG_DEVICE_NAME, 0};

    int             vid = rawVid[0] + 256 * rawVid[1];
    int             pid = rawPid[0] + 256 * rawPid[1];
    int             err;
    if((err = usbhidOpenDevice(&dev, vid, vendorName, pid, productName, 0)) != 0){
        fprintf(stderr, "error finding %s: %s\n", productName, usbErrorMessage(err));
        return NULL;
    }
    return dev;
}

void reshape(int w, int h)
{
    windowHeight = w;
    windowWidth = h;
    glViewport(0, 0, w, h);       /* Establish viewing area to cover entire window. */
    glMatrixMode(GL_PROJECTION);  /* Start modifying the projection matrix. */
    glLoadIdentity();             /* Reset project matrix. */
    glOrtho(0,w, -h/2, h/2, -1, 1);   /* Map abstract coords directly to window coords. */
}

void fetchAndRender(void)
{	
    float x[640] ={0} , y[640] = {0} ;
    long dataCount=0, plotCount= 0;
    unsigned char        tempBuffer[1+1];//[1+128/*129*/];    /* room for dummy report ID */
    int         err;
    

    int len = sizeof(tempBuffer);
 
    while(1) {
        if((err = usbhidGetReport(dev, 0, tempBuffer, &len)) != 0){                 //usbhidGetReport(usbDevice_t *device, int reportNumber, char *buffer, int *len) is defined in hiddata.h --> hiddata.c int 
            fprintf(stderr, "error reading data: %s\n", usbErrorMessage(err));
        }else{
                printf("I am dumping the hex now!:%ld\n",dataCount);
                hexdump(tempBuffer + 1, sizeof(tempBuffer) - 1);
        }

        dataCount++;      
        glColor3f(0, 1.0, 0);
        glLineWidth(0.5);
        for (int i = 0; i < 1/*28*/; ++i)               //128 is the no. of data being send from firmware
        {   
            glClear(GL_COLOR_BUFFER_BIT);
        
            glBegin(GL_LINE_STRIP);
            for (int j =0; j<=638; ++j){
                x[j] = x[j+1];y[j] = y[j+1];
                glVertex2f(x[j], y[j]);
            }
            plotCount++;
            x[639] = plotCount; y[639] = tempBuffer[i+1];
            glVertex2f(x[639],y[639]);
            glEnd();
            if (plotCount>windowWidth){
                glTranslatef(-1,0,0);        
            }
        }
        glutSwapBuffers(); /* Single buffered, so needs a flush. */
        glFlush();
    }
}

int main(int argc, char **argv)
{
    
if((dev = openDevice()) == NULL)
    exit(1);

glutInit(&argc, argv);
glutInitWindowSize(640,640);   
glutInitWindowPosition(200,200);
glutCreateWindow("Graph Window");
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutDisplayFunc(fetchAndRender);
glutReshapeFunc(reshape);
glutMainLoop();

usbhidCloseDevice(dev);
return 0;

}

/* ------------------------------------------------------------------------- */
