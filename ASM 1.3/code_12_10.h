#ifndef CODE_12_10_H
#define CODE_12_10_H

#include "hwlib.hpp"
#include <array>

class drawable {
protected:

   hwlib::glcd_oled & w;
   hwlib::xy location;
   hwlib::xy size;
   
public:
	
	hwlib::xy stuiteren;
	
   drawable( hwlib::glcd_oled & w, const hwlib::xy & location, const hwlib::xy & size,const hwlib::xy & stuiteren = hwlib::xy(1,1) ):
      w( w ),
      location( location ),
      size( size ),
	  stuiteren(stuiteren)
   {}      
   
   virtual void draw() = 0;
   virtual void update(){}
   bool overlaps( const drawable & other );   
   virtual void interact( drawable & other ){}
   
   hwlib::ostream & print( hwlib::ostream & out ) const {
      return out << location << " " << ( location + size );
   }      
};

hwlib::ostream & operator<<( hwlib::ostream & lhs, const drawable & rhs ){
   return rhs.print( lhs );
}

bool within( int x, int a, int b ){
   return ( x >= a ) && ( x <= b );
}

bool drawable::overlaps( const drawable & other ){
   
   bool x_overlap = within( 
      location.x, 
      other.location.x, 
      other.location.x + other.size.x
   ) || within( 
      other.location.x, 
      location.x, 
      location.x + size.x
   );
     
   bool y_overlap = within( 
      location.y, 
      other.location.y, 
      other.location.y + other.size.y
   ) || within( 
      other.location.y, 
      location.y, 
      location.y + size.y
   );
   
   return x_overlap && y_overlap;
}

// ===========================================================================

class line : public drawable {
protected: 

   hwlib::xy end;
   
public:

   line( hwlib::glcd_oled & w, const hwlib::xy & location, const hwlib::xy & end ):
      drawable( w, location, end - location ),
      end( end )
   {}
   
   void draw() override {
      hwlib::line x( location, end );
      x.draw( w );;
   }
};

// ===============================================================================

class wall : public drawable {
private:
	bool filed = true;
	int update_interval, update_count;
public:
	wall(hwlib::glcd_oled & w,const hwlib::xy & location,const hwlib::xy & size,const hwlib::xy & stuiteren , int update_interval):
		drawable( w, location, hwlib::xy(size.x+update_interval,size.y+update_interval),stuiteren),
		update_interval(update_interval)
	{}
	void draw() override {
		if(filed == 0){
			hwlib::line left(hwlib::xy( location.x, location.y ), hwlib::xy( location.x, size.y + 1 ));
			hwlib::line right(hwlib::xy( size.x, location.y ), hwlib::xy( size.x, size.y + 1 ));
			hwlib::line top(hwlib::xy( location.x, location.y ), hwlib::xy( size.x + 1, location.y ));
			hwlib::line bottom(hwlib::xy( location.x, size.y ), hwlib::xy( size.x + 1, size.y ));
			left.draw(w);
			right.draw(w);
			top.draw(w);
			bottom.draw(w);
			w.flush();
		} else {
			for (int i =location.y; i < size.y +1; i++){
				for(int j=location.x; j < size.x +1; j++){
					w.write(hwlib::xy(j,i),hwlib::color(255,255,255));
				}
			}
			w.flush();
		}
		
	};
	void update() override{
		if(filed == 0){
			filed = true;
		} else {
			filed = false;
		}
	};
};

// ===========================================================================

class circle : public drawable {
protected:

   int radius;
 
public:

   circle( hwlib::glcd_oled & w, const hwlib::xy & midpoint, int radius ):
      drawable( w, 
         midpoint - hwlib::xy( radius, radius ), 
         hwlib::xy( radius, radius ) * 2 ),
      radius( radius )
   {}
   
   void draw() override {
      hwlib::circle c( location + hwlib::xy( radius, radius ), radius );
      c.draw( w );
   }
};

// ===========================================================================

class ball : public circle {
private:

   hwlib::xy speed;
   
public:

   ball( 
      hwlib::glcd_oled & w, 
      const hwlib::xy & midpoint, 
      int radius, 
      const hwlib::xy & speed 
   ):
      circle( w, midpoint, radius ),
      speed( speed )  
   {}
   
   void update() override {
      location = location + speed; 
   }
   
   void interact( drawable & other ) override {
      if( this != & other){
         if( overlaps( other )){
            speed.x *= other.stuiteren.x;
            speed.y *= other.stuiteren.y;
         }
      }
   }   
};

// ===========================================================================

class victim : public drawable {
private:
	int length;
public:
   victim( hwlib::glcd_oled & w,const hwlib::xy & location,int length ):
		drawable( w,location- hwlib::xy( length, length ), hwlib::xy( length, length ) * 2),length( length )
	{}
   void draw() override{
		if(length >= 0){
			line left (w,hwlib::xy(location.x-length,location.y-length),hwlib::xy(location.x-length,location.y+length+1));
			line right(w,hwlib::xy(location.x+length,location.y-length),hwlib::xy(location.x+length,location.y+length+1));
			line top(w,hwlib::xy(location.x-length,location.y+length),hwlib::xy(location.x+length+1,location.y+length));
			line bottom(w,hwlib::xy(location.x-length,location.y-length),hwlib::xy(location.x+length+1,location.y-length));
			left.draw();
			right.draw();
			top.draw();
			bottom.draw();
		} else {
			location = hwlib::xy(0,0);
		}
		w.flush();
	}
	
//	void update() override{}
	void interact( drawable & other ) override {
		if( this != & other){
			if( overlaps( other )){
				if (length >= 0){
					length -= 1;
				}
			} 
		}
	}   
};

#endif