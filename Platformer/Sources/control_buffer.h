#include <map>

#define N_CONTROLS 11

/* Control buffer
R: right
L: left
U: up
D: down
+: zoom in
-: zoom out
1: draw weapon 1
2: draw weapon 2
S: shoot
r: reload
G: replay
*/

extern std::map<char, bool> control_buffer;
extern std::map<char, bool> release_buffer;

extern char control_key [N_CONTROLS];
extern unsigned int control_key_size;

extern float mouse_dx, mouse_dy;
