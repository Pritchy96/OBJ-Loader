// TestImageLoad.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "EasyBMP.h"
using namespace std;

int wasMain(int argc, char* argv[])
{  
 BMP heightmap;
 heightmap.ReadFromFile("heightmap.bmp");
  
 BMP Output;
 Output.SetSize( heightmap.TellWidth() , heightmap.TellHeight() );
 Output.SetBitDepth( 24 );
 
 for (int i = 0; i < heightmap.TellWidth(); i++)
 {
	 for (int j = 0; j < heightmap.TellHeight(); j++)
	{
		RGBApixel pixel = RGBApixel();
		pixel.Blue = heightmap.GetPixel(i, j).Blue;
		pixel.Red = heightmap.GetPixel(i, j).Blue;
		pixel.Green = heightmap.GetPixel(i, j).Blue;

		Output.SetPixel(i, j, pixel);
	}
 }
						 				
 Output.SetBitDepth( 32 );
 cout << "writing Image ... " << endl;					
 Output.WriteToFile( "BWTEST.bmp" );

 return 0;
}

/*

 Output.SetBitDepth( 24 );
 cout << "writing 24bpp ... " << endl;			
 Output.WriteToFile( "EasyBMPoutput24bpp.bmp" );
 
 Output.SetBitDepth( 8 );
 cout << "writing 8bpp ... " << endl;					
 Output.WriteToFile( "EasyBMPoutput8bpp.bmp" );

 Output.SetBitDepth( 4 );
 cout << "writing 4bpp ... " << endl;					
 Output.WriteToFile( "EasyBMPoutput4bpp.bmp" );
 
 Output.SetBitDepth( 1 );
 cout << "writing 1bpp ... " << endl;					
 Output.WriteToFile( "EasyBMPoutput1bpp.bmp" );
 
 Output.SetBitDepth( 24 );
 Rescale( Output, 'p' , 50 );
 cout << "writing 24bpp scaled image ..." << endl;
 Output.WriteToFile( "EasyBMPoutput24bpp_rescaled.bmp" );*/