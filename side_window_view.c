#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define ANIMATION_COOLDOWN 120000

int h = 10;
int w = 35;

int Abs(int n) {
    return n > 0 ? n : -n;
}

int Coord_To_Byte(int y, int x) {
    return (x-1)+(y-1)*(w+1);
}

void Set_Coord_char(char *output, int y, int x, char c){
    output[Coord_To_Byte(y,x)] = c;
}

void pSet_Coord_char(char *output, int y, int x, char c, int n) {
    // plural version of setting coords to char
    for(int i = 0; i < n; i++) {
        output[Coord_To_Byte(y,x)+i] = c;
    }
}

void Shift_char(char *output, int y, int x, int dy, int dx) {
    // if position after shift is out then just clear previous pos

    //if shift destination has same char just do nothing i guess?
    if(output[Coord_To_Byte(y+dy,x+dx)] != output[Coord_To_Byte(y,x)]) {
        if(y + dy <= h && y + dy >= 1){
            if(x + dx <= w && x + dx >= 1) {
                // if des is within grid and not the same char
                // shift char
                output[Coord_To_Byte(y+dy,x+dx)] = output[Coord_To_Byte(y,x)];
            }        
        }
    }
    output[Coord_To_Byte(y,x)] = ' ';
}
int clearSlots;
void pShift_char(char *output, int y, int x, int n, int dx){
    int startByte = Coord_To_Byte(y,x);
    for(int i = 0; i < n; i++) {
        if(dx > 0){
            output[startByte+n-1+dx-i] = output[startByte+n-1-i];
        }else{
            output[startByte+i+dx] = output[startByte+i];
        }
    }
    
    if(Abs(dx) < n)
        clearSlots = Abs(dx);
    else
        clearSlots = n - Abs(dx);

    
    if(dx > 0){
        for(int i = 0; i < clearSlots; i++) {
            output[startByte+i] = ' ';
        }
    }else{
        for(int i = 0; i < clearSlots; i++) {
            output[startByte+n-1-i] = ' ';
        }
    }
}

//making this a func since used for ground and stars
char rand_chance_char(char common, char special, int percent){
    if(rand() % 100 < percent)
        return special;
    else
        return common;
}

int main() {
    srand(time(NULL));

    //Ads Columns
    int outputBytes = (w+1) * h;
    char output[outputBytes];
    // fill the array with ' '
    memset(output, ' ', (outputBytes-1)*sizeof(char));
    output[outputBytes-1] = '\0';
    // adding colums
    for(int i = 1; i < h; i++){
        output[i*(w+1)-1] = '\n';
    }

    /****************************************************************************
    
    Drawing Ground
    
    ****************************************************************************/
    for(int i = 0; i < w-6; i++) {
        Set_Coord_char(output,h-1,5+i,rand_chance_char('-','.',40));
    }

    /****************************************************************************

    Drawing Trees

    ****************************************************************************/

        /****************************************************************************
            S is space
            - Big Tree
            1234567
               ^
              /S\
             /SSS\
            /_____\
               ||
            - small tree
            12345
              ^
             /S\
            /___\
              |

        ****************************************************************************/

    void Big_Tree_Column(int x,int index){
        if(x >= w || x <= 0)
            return;
        switch (index) {
        case 1:
            Set_Coord_char(output,h-3,x,'/');
            break;
        case 2:
            Set_Coord_char(output,h-4,x,'/');
            Set_Coord_char(output,h-3,x,'_');
            break;
        case 3:
            Set_Coord_char(output,h-5,x,'/');
            Set_Coord_char(output,h-4,x,' ');
            Set_Coord_char(output,h-3,x,'_');
            break;
        case 4:
            Set_Coord_char(output,h-6,x,'^');
            Set_Coord_char(output,h-5,x,' ');
            Set_Coord_char(output,h-4,x,' ');
            Set_Coord_char(output,h-3,x,'_');
            Set_Coord_char(output,h-2,x,'|');
            break;
        case 5:
            Set_Coord_char(output,h-5,x,'\\');
            Set_Coord_char(output,h-4,x,' ');
            Set_Coord_char(output,h-3,x,'_');
            Set_Coord_char(output,h-2,x,'|');
            break;
        case 6:
            Set_Coord_char(output,h-4,x,'\\');
            Set_Coord_char(output,h-3,x,'_');
            break;
        case 7:
            Set_Coord_char(output,h-3,x,'\\');
            break;
            
        default:
            break;
        }
    }

    void Small_Tree_Column(int x,int index){
        if(x >= w || x <= 0)
            return;
        switch (index) {
        case 1:
            Set_Coord_char(output,h-3,x,'/');
            break;
        case 2:
            Set_Coord_char(output,h-4,x,'/');
            Set_Coord_char(output,h-3,x,'_');
            break;
        case 3:
            Set_Coord_char(output,h-5,x,'^');
            Set_Coord_char(output,h-4,x,' ');
            Set_Coord_char(output,h-3,x,'_');
            Set_Coord_char(output,h-2,x,'|');
            break;
        case 4:
            Set_Coord_char(output,h-4,x,'\\');
            Set_Coord_char(output,h-3,x,'_');
            break;
        case 5:
            Set_Coord_char(output,h-3,x,'\\');
            break;
        default:
            break;
        }
    }

    // plant small tree
    void PSmall_Tree(int treePos){
        Small_Tree_Column(treePos-1,1);
        Small_Tree_Column(treePos,2);
        Small_Tree_Column(treePos+1,3);
        Small_Tree_Column(treePos+2,4);
        Small_Tree_Column(treePos+3,5);
    }

    void PBig_Tree(int treePos){
        Big_Tree_Column(treePos-3,1);
        Big_Tree_Column(treePos-2,2);
        Big_Tree_Column(treePos-1,3);
        Big_Tree_Column(treePos,4);
        Big_Tree_Column(treePos+1,5);
        Big_Tree_Column(treePos+2,6);
        Big_Tree_Column(treePos+3,7);

    }

    /****************************************************************************
    
      Drawing Border
    
    ****************************************************************************/
        void Draw_Border(){
            for(int i = 1; i < h; i++) {
                // left corner
                pSet_Coord_char(output, i,1,' ', h-i);
                // diagonal line
                Set_Coord_char(output, i+1,h-i,'/');
                // Right Rim
                Set_Coord_char(output,i+1,w,'|');
            }
        }
        Draw_Border();
        // top ceiling
        pSet_Coord_char(output, 1,h, '_',w-h+1);
        
        // bottom rim
        pSet_Coord_char(output,h,2,'_',w-2);

        for(int i = 0; i < w-11;i++){
            Set_Coord_char(output, 2, 10+i, rand_chance_char(' ',rand_chance_char('.','+',20),20));
        }
        for(int i = 0; i < w-10;i++){
            Set_Coord_char(output, 3, 9+i, rand_chance_char(' ',rand_chance_char('.','+',20),20));
        }

    
    printf("\033[H\033[J"); // clears screen

    // I do not like this part
    int BigTreePos[] = {1,12,20,25};
    int SmallTreePos[] = {6,15,24};
    int n = 1;
    char *msgs[] = {"Are we there yet?", "not yet"};
    int m = 0;
    while(1){


        for(int i = 0; i < 3; i++){
            PBig_Tree(BigTreePos[i]);

            BigTreePos[i]++;
    
            if(BigTreePos[i] >= w || BigTreePos[i] < 0 )
                BigTreePos[i] = 3;

        }

        for(int i = 0; i < 3; i++){
            PSmall_Tree(SmallTreePos[i]);
    
            SmallTreePos[i]++;
    
            if(SmallTreePos[i] >= w || SmallTreePos[i] < 0 )
                SmallTreePos[i] = 3;

        }

        Draw_Border();

        //shift whole area withing frame
        for(int i = 1; i <= h-4; i++)
            pShift_char(output,h-i,2+i,w-3-i,1);

        // shift sky only every 7 shifts
        if(n % 7 == 0){
            pShift_char(output,2,10,24,1);
            pShift_char(output,3,9,25,1);
            Set_Coord_char(output, 2, 10, rand_chance_char(' ',rand_chance_char('.','+',10),10));
            Set_Coord_char(output, 3, 9, rand_chance_char(' ',rand_chance_char('.','+',10),10));
        }
        if(n % 60 == 0){
            m ^= 1;     // switches through 1 and 0 with XOR
        }
        n++;
        
        //h-1, 2nd line from bottom, from 5 is going forward, w-2 if backward
        Set_Coord_char(output,h-1,5,rand_chance_char('-','.',40));
        usleep(ANIMATION_COOLDOWN);
        printf("%s\n\n",output);
        printf("   %s\n",msgs[m]);
        printf("\033[1J\033[H"); // clears screen
    }
}
