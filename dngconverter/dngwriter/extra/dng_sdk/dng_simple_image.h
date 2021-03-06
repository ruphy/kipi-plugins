/*****************************************************************************/
// Copyright 2006-2008 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

/* $Id: //mondo/dng_sdk_1_2/dng_sdk/source/dng_simple_image.h#1 $ */ 
/* $DateTime: 2008/03/09 14:29:54 $ */
/* $Change: 431850 $ */
/* $Author: tknoll $ */

/*****************************************************************************/

#ifndef __dng_simple_image__
#define __dng_simple_image__

/*****************************************************************************/

#include "dng_auto_ptr.h"
#include "dng_image.h"
#include "dng_pixel_buffer.h"

/*****************************************************************************/

/// dng_image derived class with simple Trim and Rotate functionality.
 
class dng_simple_image : public dng_image
	{
	
	protected:
	
		dng_pixel_buffer fBuffer;
		
		AutoPtr<dng_memory_block> fMemory;
		
	public:
	
		dng_simple_image (const dng_rect &bounds,
				  		  uint32 planes,
				  		  uint32 pixelType,
				  		  uint32 pixelRange,
				  		  dng_memory_allocator &allocator);
		
		virtual ~dng_simple_image ();
	
		/// Setter for pixel type.
		
		virtual void SetPixelType (uint32 pixelType);
		
		/// Setter for pixel range.
		
		virtual void SetPixelRange (uint32 pixelRange);

		/// Trim image data outside of given bounds. Memory is not reallocated or freed.

		virtual void Trim (const dng_rect &r);

		/// Rotate image according to orientation.
		
		virtual void Rotate (const dng_orientation &orientation);
		
		/// Get the buffer for direct processing. (Unique to dng_simple_image.)
		
		void GetPixelBuffer (dng_pixel_buffer &buffer)
			{
			
			buffer = fBuffer;
			
			}

	protected:
	
		virtual void AcquireTileBuffer (dng_tile_buffer &buffer,
										const dng_rect &area,
										bool dirty) const;
		
	};

/*****************************************************************************/

#endif
	
/*****************************************************************************/
