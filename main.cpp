#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>
#include<ctype.h>

#define pi (2*acos(0.0))

#define sqrt2 (sqrt(2))

typedef struct point
{
	double x,y,z;
}Point;
bool windfunc = false;
bool doorfunc = false;
bool drawAxes = true;
float tyreAngle = 90;
float Door = 0;
float halfdoor =0;

float RotateHouse = 0;
float RotateExhaust = 0;
float fan = 0;
float halfwindow = 0;
float quadAngle = 0;
float boxAngle = 0;
float houseScaleFactor = 1;
float tyreScaleFactor = 1;
float tinyBoxAngle = 0;
float background = 0;
float win = 0;
float slide = false;

Point pos, u, r, l;

void displayAxes()
{
	if(drawAxes)
	{
		glBegin(GL_LINES);
		{
		    glColor3f(1.0, 0, 0);

			glVertex3f( 500,0,0); // a point on pos x axis
			glVertex3f(-500,0,0); // a point on neg x axis

			glColor3f(0, 1.0, 0);

			glVertex3f(0,-500,0); // a point on pos y axis
			glVertex3f(0, 500,0); // a point on neg y axis

			glColor3f(0, 0, 1.0);

			glVertex3f(0,0, 500);  // a point on pos z axis
			glVertex3f(0,0,-500);   // a point on neg z axis
		}
		glEnd();
	}
}

Point func(Point vect, Point perp, int dir)
{
    double c = cos(pi/180);
    double s = dir * sin(pi/180);
    Point point;
    point.x = c * vect.x + s * perp.x;
    point.y = c * vect.y + s * perp.y;
    point.z = c * vect.z + s * perp.z;
    c = sqrt(point.x*point.x + point.y*point.y + point.z*point.z);
    point.x /= c;
    point.y /= c;
    point.z /= c;
    return point;
}

Point position_func(Point vect, Point position, int dir){
    Point point;
    point.x = dir * 3 * vect.x + position.x;
    point.y = dir * 3 * vect.y + position.y;
    point.z = dir * 3 * vect.z + position.z;
    return point;
}

void keyboardListener(unsigned char key, int x,int y){
    key = toupper(key);
	switch(key){
		case '1': {
            Point l1 = func(l, r, -1);
            r = func(r, l, 1);
            l = l1;
            break;
		}
		case '2': {
            Point l1 = func(l, r, 1);
            r = func(r, l, -1);
            l = l1;
            break;
		}

		case '3': {
            Point u1 = func(u, l, -1);
            l = func(l, u, 1);
            u = u1;
            break;
        }
        case '4': {
            Point u1 = func(u, l, 1);
            l = func(l, u, -1);
            u = u1;
            break;
        }
        case '5': {
            Point r1 = func(r, u, -1);
            u = func(u, r, 1);
            r = r1;
            break;
        }
        case '6':{
            Point r1 = func(r, u, 1);
            u = func(u, r, -1);
            r = r1;
            break;
        }
//        case '7':{
//            tyreScaleFactor += 0.2;
//            break;
//        }
//        case '8':{
//            tyreScaleFactor -= 0.2;
//            break;
//        }
        case '9':{
           if(win>-60){
                win -= 10;
            }
            break;
        }
        case '0':{
            if(win<0){
                win += 10;
           }
            break;
        }
        case 'U':{
            if(houseScaleFactor<2){
                    houseScaleFactor += 0.2;
                    break;
                }
        }
        case 'D':{
            if(houseScaleFactor>0){
                    houseScaleFactor -= 0.2;
                    break;
                }
        }

        case 'L':{
                    RotateHouse -= 5;
                    break;
        }
        case 'R':{
                    RotateHouse += 5;
                    break;
        }

		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN: // down arrow
		    {
		        pos = position_func(l, pos, -1);
		        break;
		    }

		case GLUT_KEY_UP:   // up arrow
		    {
		        pos = position_func(l, pos, 1);
		        break;
		    }

		case GLUT_KEY_RIGHT:    // right arrow
		    {
		        pos = position_func(r, pos, 1);
		        break;
		    }

		case GLUT_KEY_LEFT:     // left arrow
		    {
		        pos = position_func(r, pos, -1);
		        break;
		    }

        case GLUT_KEY_PAGE_UP:
		    {
                pos = position_func(u, pos, 1);
                break;
		    }

		case GLUT_KEY_PAGE_DOWN:
		    {
		       pos = position_func(u, pos, -1);
                break;
		    }

		case GLUT_KEY_HOME:
		    {
		        if(Door<=50){
                    Door += 5;
		        }
		        break;
		    }

        case GLUT_KEY_END:
            {   if(Door>0){
                    Door -= 5;
                }
                break;
            }

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN) // 2 times?? in ONE click? -- solution is checking DOWN or UP
            {
                printf("Mouse Left Button Clicked\n");
			}
			else if(state == GLUT_UP)
            {
                printf("Mouse Left Button Released\n");
            }
            break;
		default:
			break;
	}
}

void displayTyre(float tyreRadius, float tyreWidth)
{
    int tyreSlices = 10;
    float sliceWidth = (2 * M_PI * tyreRadius) / tyreSlices;
    float sliceAngle = 360 / tyreSlices;

    float halfTyreWidth = tyreWidth / 2, quartTyreWidth = halfTyreWidth / 2;
    float halfSliceWidth = sliceWidth / 2;

    for(int i=0; i<tyreSlices; i++)
    {
        if(i%2 == 0){
            glColor3f(0, 0, 0);
        }else{
            glColor3f(1, 1, 1);
        }
        glPushMatrix();
        glRotatef(sliceAngle*i, 1, 0, 0);
        glTranslatef(0, tyreRadius, 0);
        glBegin(GL_QUADS);
        {
            glVertex3f(halfTyreWidth, 0, halfSliceWidth);
            glVertex3f(halfTyreWidth, 0, -halfSliceWidth);
            glVertex3f(-halfTyreWidth, 0, -halfSliceWidth);
            glVertex3f(-halfTyreWidth, 0, halfSliceWidth);
        }
        glEnd();
        glPopMatrix();


    }
}

void displayQuad(float tyreRadius, float smalltyreRadius, float tyreWidth) // width along x axis, length along z axis
{
    float half_fanLength = (tyreRadius-smalltyreRadius)/2, halfTyreWidth = tyreWidth / 2;


    int tyreSlices = 10;
    float sliceWidth = (2 * M_PI * tyreRadius) / tyreSlices;
    float sliceAngle = 360 / tyreSlices;

    float halfSliceWidth = sliceWidth / 2;

    for(int i=0; i<tyreSlices; i++)
    {
        if(i%2 ==0 ){
            glColor3f(0.25, 0.5, 1);
        }else{
            glColor3f(1, 0, 0);
        }
        glPushMatrix();
        glRotatef(sliceAngle*i, 1, 0, 0);
        glTranslatef(0, smalltyreRadius+half_fanLength, 0);
        glBegin(GL_QUADS);
        {
            glVertex3f(halfTyreWidth, half_fanLength, 0);
            glVertex3f(-halfTyreWidth, half_fanLength, 0);
            glVertex3f(-halfTyreWidth, -half_fanLength, 0);
            glVertex3f(halfTyreWidth, -half_fanLength, 0);
        }
        glEnd();
        glPopMatrix();


    }

}

void displayDoor(float doorLength, float doorHeight){

    float halfDoorLength = doorLength/2, halfDoorHeight = doorHeight/2;
    glBegin(GL_QUADS);
    {
    glColor3f(0, 0.6, 0.52156862745);

    glVertex3f(halfDoorLength, 0, halfDoorHeight);
    glColor3f(0.819607, 0.862745, 0.9294117);
    glVertex3f(-halfDoorLength, 0, halfDoorHeight);
    glVertex3f(-halfDoorLength, 0, -halfDoorHeight);
    glVertex3f(halfDoorLength, 0, -halfDoorHeight);
    }
    glEnd();

}

void displayBox(float boxLength, float boxWidth, float boxHeight, int c) // length -> x axis, width -> yaxis, height -> z axis
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;
    glBegin(GL_QUADS);
    {   if(c == 1){
            glColor3f(0.8, 0.8, 0.792157);
        }else if(c == 2){
            glColor3f(0.015686, 0.8, 0.40784);
        }else if(c == 3){
            glColor3f(0, 0.862745, 0.90980);
        }else if(c == 4){
            glColor3f(0.796078, 0.83922, 0);
        }else if (c == 5){
            glColor3f(0.90980, 0.18824, 0.137255);
        }else if ( c == 6){
            glColor3f(0.121568, 0.811764, 0.603921);
        }
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);


         if(c == 1){
            glColor3f(0.741176, 0.74902, 0.713725);
        }else if(c == 2){
            glColor3f(0.05098, 0.819608, 0.43529);
        }else if(c == 3){
            glColor3f(0.03529, 0.84706, 0.89019);
        }else if(c == 4){
            glColor3f(0.788235, 0.831372, 0.031372);
        }else if (c == 5){
            glColor3f(0.878431, 0.117647, 0.062745);
        }else if ( c == 6){
            glColor3f(0.054902, 0.788235, 0.568627);
        }

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);

         if(c == 1){
            glColor3f(0.77255, 0.78039, 0.7333);
        }else if(c == 2){
            glColor3f(0.02353, 0.8, 0.411765);
        }else if(c == 3){
            glColor3f(0.058823, 0.8, 0.83922);
        }else if(c == 4){
            glColor3f(0.827451, 0.870588, 0.050980);
        }else if (c == 5){
            glColor3f(0.941176, 0.098039, 0.039216);
        }else if ( c == 6){
            glColor3f(0.015686, 0.760784, 0.537255);
        }

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
    }
    glEnd();
}

void displayScene()
{
    //Primitive Object dimensions

    float quadLength = 100, quadWidth = 60;

    float baseLength = 300, baseWidth = 150, baseHeight = 24; //house base

    float bodyLenght = 280, bodyWidth = 130, bodyHeight = 200; //house body

    float doorLength = 55, doorHeight = 150; //door

    float upDoorLength = 110, upDoorWidth = 20, upDoorHeight = 20; //door ceiling

    float windowWidth = 50, windowHeight = 70;

    float upWindowLength = 50, upWindowWidth = 15, upWindowHeight = 15;

    float roofLength = 300, roofWidth = 150, roofHeight = 12; // house roof

    float topLength = 100, topWidth = 50, topHeight = 120; // house ventilator

    float stair1Lenght= 150, stair1Width = 120, stair1Height = 8; //stair1

    float stair2Lenght= 130, stair2Width = 100, stair2Height = 8; //stair 2

    float stair3Lenght= 110, stair3Width = 80, stair3Height = 8; //stair 3

    float tyreRadius = 40, tyreWidth = 20, smalltyreRadius = 20;



    /** Principal Axes **/

    displayAxes();


    /** Animate Function **/
    glPushMatrix();

    glRotatef(RotateHouse,0,0,1);

    glScalef(houseScaleFactor, houseScaleFactor, houseScaleFactor);

        glPushMatrix();
        displayBox(baseLength, baseWidth, baseHeight, 1); // base
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, 0, (bodyHeight/2) + (baseHeight/2)); // places the body on top of the base
        displayBox(bodyLenght, bodyWidth, bodyHeight, 2); // body
        glPopMatrix();

        glPushMatrix();
        glTranslatef(+Door, 0, 0);
        glTranslatef((doorLength/2), (bodyWidth/2)+1, (baseHeight/2)+(doorHeight/2)); // places the door on side of the body
        displayDoor(doorLength, doorHeight); // front leftdoor
        glPopMatrix();


        glPushMatrix();
        glTranslatef(-Door, 0, 0);
        glTranslatef((-doorLength/2), (bodyWidth/2)+1, (baseHeight/2)+(doorHeight/2)); // places the door on side of the body
        displayDoor(doorLength, doorHeight); // front rightdoor
        glPopMatrix();


        glPushMatrix();
        glTranslatef(-doorLength, -(bodyWidth/2)-1, (baseHeight/2)+(doorHeight/2)); // places the door on side of the body
        glRotatef(-halfdoor, 0, 0, 1);
        glTranslatef((doorLength/2),0,0);
        displayDoor(doorLength, doorHeight); // back leftdoor
        glPopMatrix();

        glPushMatrix();
        glTranslatef(doorLength, -(bodyWidth/2)-1, (baseHeight/2)+(doorHeight/2)); // places the door on side of the body
        glRotatef(+halfdoor, 0, 0, 1);
        glTranslatef((-doorLength/2),0,0);
        displayDoor(doorLength, doorHeight); // back rightdoor
        glPopMatrix();
//
        glPushMatrix();
        glTranslatef(0, (bodyWidth/2)+1, doorHeight+(upDoorHeight/2) + (baseHeight/2)); // places the body on top of the base
        displayBox(upDoorLength, upDoorWidth, upDoorHeight, 5); // front door celing
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, -(bodyWidth/2)-1, doorHeight+(upDoorHeight/2) + (baseHeight/2)); // places the body on top of the base
        displayBox(upDoorLength, upDoorWidth, upDoorHeight, 5); // back door celing
        glPopMatrix();

        glPushMatrix();
        glTranslatef((bodyLenght/2)+1, 0, (baseHeight/2)+(1.5*windowHeight));// places the door on side of the body
        glRotatef(win, 0, 1, 0);
        glTranslatef(0, 0,-(windowHeight/2));
        glRotatef(90, 0, 0, 1);
        displayDoor(windowWidth, windowHeight); // left window
        glPopMatrix();


        glPushMatrix();
        glTranslatef(0,0,halfwindow);
        glTranslatef(-(bodyLenght/2)-1, 0, (baseHeight/2)+(windowHeight));// places the door on side of the body
        glRotatef(90, 0, 0, 1);
        displayDoor(windowWidth, windowHeight); // right window
        glPopMatrix();


        glPushMatrix();
        glTranslatef((bodyLenght/2)+1, 0, windowHeight+(windowHeight/2)+(upWindowHeight/2) + (baseHeight/2)); // places the body on top of the base
        glRotated(90, 0, 0, 1);
        displayBox(upWindowLength, upWindowWidth, upWindowHeight, 5); // left window celing
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-(bodyLenght/2)-1, 0, windowHeight+(windowHeight/2)+(upWindowHeight/2) + (baseHeight/2)); // places the body on top of the base
        glRotated(90, 0, 0, 1);
        displayBox(upWindowLength, upWindowWidth, upWindowHeight, 5); // right window celing
        glPopMatrix();


        glPushMatrix();
        glTranslatef(0, 0, (roofHeight/2)+bodyHeight+(baseHeight/2)); // places the rooftop on top of the body
        displayBox(roofLength, roofWidth, roofHeight, 3);  //rooftop
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, 0, (topHeight/2)+roofHeight+bodyHeight+(baseHeight/2)); // places the top on top of the rooftop
        displayBox(topLength, topWidth, topHeight, 4);  //top
        glPopMatrix();

        //start stairs
        glPushMatrix();
        glTranslatef(0, (baseWidth/2), -((baseHeight/2)-stair1Height/2)); // places the stair1 side of the base
        displayBox(stair1Lenght, stair1Width, stair1Height, 5); // stair1
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, (baseWidth/2), 0); // places the stair2 side of the base and on top of the stair1
        displayBox(stair2Lenght, stair2Width, stair2Height, 6); // stair2
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, (baseWidth/2),stair2Height-1); // places the stair3 side of the base and on top of the stair2
        displayBox(stair3Lenght, stair3Width, stair3Height, 4); // stair3
        glPopMatrix();
        //end stairs

        //Start Exhaust Fan
        glPushMatrix();
        glTranslatef(0, topWidth, (topHeight/2)+roofHeight+bodyHeight+(baseHeight/2));
        glRotatef(RotateExhaust,0,1,0);

            glPushMatrix(); // places the tyre on top of both boxes
            glRotatef(tyreAngle, 0, 0, 1); // check what happens to the tyre after exchanging the order of translation and roation
            glScalef(tyreScaleFactor, tyreScaleFactor, tyreScaleFactor);
            displayTyre(tyreRadius, tyreWidth); // big tyre
            glPopMatrix();

            glPushMatrix();
            glRotatef(tyreAngle, 0, 0, 1); // check what happens to the tyre after exchanging the order of translation and roation
            glScalef(tyreScaleFactor, tyreScaleFactor, tyreScaleFactor);
            displayTyre(smalltyreRadius, tyreWidth); // small tyre
            glPopMatrix();

            glPushMatrix();
            glRotatef(tyreAngle, 0, 0, 1); // check what happens to the tyre after exchanging the order of translation and roation
            glScalef(tyreScaleFactor, tyreScaleFactor, tyreScaleFactor);
            displayQuad(tyreRadius, smalltyreRadius, tyreWidth); //fan of the tyre
            glPopMatrix();

        glPopMatrix();
        //end Exhaust Fan

    glPopMatrix();
}

void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(background, background, background,0);	//sets background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // sets the current matrix to identity
    gluLookAt(pos.x, pos.y, pos.z, pos.x+l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z); // sets camera position and direction

    displayScene();

	glutSwapBuffers();
}



void animate(){

    if(doorfunc){
        if(halfdoor<120){
                    halfdoor += 0.05;
                }
    }

    if(!doorfunc){
                if(halfdoor>0){
                    halfdoor -= 0.05;
                }
    }

    if(windfunc){
        if(halfwindow<50){
            halfwindow += 0.05;
        }
    }
    if(!windfunc){
        if(halfwindow>0){
            halfwindow -= 0.05;
        }
    }

    if(fan == 5){
        RotateExhaust -= 0.2;
    }
    if(fan == 6){
        RotateExhaust += 0.2;
    }
    if(fan == 0){
        RotateExhaust = 0;
    }
	glutPostRedisplay();
}

void init(){
    glClearColor(background, background, background,0); 	//sets background color

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity(); // sets the current matrix to identity
    gluPerspective(80,	1,	1,	1000.0);

	// to understand the following values, read the specification pdf
    double d = 1 / sqrt2;
    pos = {300, 300, 150};
    u = {0, 0, 1};
    r = {-d, d, 0};
    l = {-d, -d, 0};
}

void subMenu(int id)
{
    if(id == 3)
    {
        background = 1;
    }
    else if(id == 4)
    {
        background = 0;
    }
}

void exh(int id){
    if(id == 5){
        fan = 5;
    }else if(id == 6){
        fan = 6;
    }else if(id == 7){
        fan = 0;
    }
}

void wind(int id){
    if (id == 8){
        windfunc = true;
    }else if(id == 9){
        windfunc = false;
    }
}

void dor(int id){
    if(id == 11 ){
        doorfunc = true;
    }else if(id==12){
        doorfunc = false;
    }
}

void mainMenu(int id)
{
    if(id == 1)
    {
        drawAxes = true;
    }
    else if(id == 2)
    {
        drawAxes = false;
    }
    else if(id == 3){
        houseScaleFactor = 1;
    }
}


int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	int subMenuNo = glutCreateMenu(subMenu);
    glutAddMenuEntry("White", 3);
	glutAddMenuEntry("Black", 4);

//    int subMenuNo2 = glutCreateMenu();
    int Exhaust = glutCreateMenu(exh);
    glutAddMenuEntry("Clockwise",5);
    glutAddMenuEntry("Anti-clockwise",6);
    glutAddMenuEntry("Stop",7);

    int window = glutCreateMenu(wind);
    glutAddMenuEntry("Slide Up",8);
    glutAddMenuEntry("Slide Down",9);

    int door = glutCreateMenu(dor);
    glutAddMenuEntry("Open", 11);
    glutAddMenuEntry("Close", 12);

    glutCreateMenu(mainMenu);
	glutAddMenuEntry("Axes On", 1);
	glutAddMenuEntry("Axes Off", 2);

	glutAddMenuEntry("Original Size", 3);
	glutAddSubMenu("Background", subMenuNo);
	glutAddSubMenu("Exhaust Fan", Exhaust);
	glutAddSubMenu("Sliding Window", window);
	glutAddSubMenu("Rotating Door", door);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	//The main loop of OpenGL, this function never returns

	return 0;
}
