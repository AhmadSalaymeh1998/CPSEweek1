#include "hwlib.hpp"

extern "C" void print_asciz(const char *c);

extern "C" void uart_put_char( char c ){
   hwlib::cout << c;
}

//void print_asciz( const char * s ){
//   while( *s != '\0'){
//      uart_put_char( *s );
//      ++s;
//   }
//}
extern "C" void toggle();
extern "C" void application();

int main( void ){	
   
   namespace target = hwlib::target;   
    
   // wait for the PC console to start
   hwlib::wait_ms( 2000 );

   application();
}