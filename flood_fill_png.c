#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>

void abort_(const char * s, ...)
{
        va_list args;
        va_start(args, s);
        vfprintf(stderr, s, args);
        fprintf(stderr, "\n");
        va_end(args);
        abort();
}

int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

void read_png_file(char* file_name)
{
        char header[8];    // 8 is the maximum size that can be checked

        /* open file and test for it being a png */
        FILE *fp = fopen(file_name, "rb");
        if (!fp)
                abort_("[read_png_file] File %s could not be opened for reading", file_name);
        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8))
                abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);


        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
                abort_("[read_png_file] png_create_read_struct failed");

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
                abort_("[read_png_file] png_create_info_struct failed");

        if (setjmp(png_jmpbuf(png_ptr)))
                abort_("[read_png_file] Error during init_io");

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);


        /* read file */
        if (setjmp(png_jmpbuf(png_ptr)))
                abort_("[read_png_file] Error during read_image");

        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (y=0; y<height; y++)
                row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

        png_read_image(png_ptr, row_pointers);

        fclose(fp);
}


void write_png_file(char* file_name)
{
        /* create file */
        FILE *fp = fopen(file_name, "wb");
        if (!fp)
                abort_("[write_png_file] File %s could not be opened for writing", file_name);


        /* initialize stuff */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
                abort_("[write_png_file] png_create_write_struct failed");

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
                abort_("[write_png_file] png_create_info_struct failed");

        if (setjmp(png_jmpbuf(png_ptr)))
                abort_("[write_png_file] Error during init_io");

        png_init_io(png_ptr, fp);


        /* write header */
        if (setjmp(png_jmpbuf(png_ptr)))
                abort_("[write_png_file] Error during writing header");

        png_set_IHDR(png_ptr, info_ptr, width, height,
                     bit_depth, color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);


        /* write bytes */
        if (setjmp(png_jmpbuf(png_ptr)))
                abort_("[write_png_file] Error during writing bytes");

        png_write_image(png_ptr, row_pointers);


        /* end write */
        if (setjmp(png_jmpbuf(png_ptr)))
                abort_("[write_png_file] Error during end of write");

        png_write_end(png_ptr, NULL);

        /* cleanup heap allocation */
        for (y=0; y<height; y++)
                free(row_pointers[y]);
        free(row_pointers);

        fclose(fp);
}


void flood_fill(int node_x, int node_y, int R_val, int G_val, int B_val, int A_val, int R_target, int G_target, int B_target, int A_target) {

	// lets make sure we don't step out of bounds
	if (node_y >= height || node_y < 0) return;
	if (node_x >= width || node_x < 0) return;

	png_byte* node_row = row_pointers[node_y];
	png_byte* node_ptr = &(node_row[node_x*4]);



	// 1. If replacement color is equal to current pixel color, return.
	if (((node_ptr[0] == R_target) && (node_ptr[1] == G_target)) && ((node_ptr[2] == B_target) && (node_ptr[3] == A_target))){
		
		return;
	
	
	// 2. ElseIf the color of the pixel(node) is not equal to target-color, return.
	} else if(((node_ptr[0] != R_val) || (node_ptr[1] != G_val)) || ((node_ptr[2] != B_val) || (node_ptr[3] != A_val))){

		return;	
	
	
	// 3. Else Set the color of node to replacement-color.
	} else {
	
		node_ptr[0] = R_target;			
		node_ptr[1] = G_target;			
		node_ptr[2] = B_target;			
		node_ptr[3] = A_target;			
	
	
		//printf("X:%i Y:%i\n replaced", node_x,node_y);
	}
	
	/*
	   4. Perform Flood-fill (one step to the south of node, target-color, replacement-color).
	      Perform Flood-fill (one step to the north of node, target-color, replacement-color).
	      Perform Flood-fill (one step to the west of node, target-color, replacement-color).
	      Perform Flood-fill (one step to the east of node, target-color, replacement-color).
	*/

	flood_fill(node_x,node_y+1,R_val,G_val,B_val,A_val,R_target,G_target,B_target,A_target); //North
	flood_fill(node_x,node_y-1,R_val,G_val,B_val,A_val,R_target,G_target,B_target,A_target); //South
	flood_fill(node_x-1,node_y,R_val,G_val,B_val,A_val,R_target,G_target,B_target,A_target); //West
	flood_fill(node_x+1,node_y,R_val,G_val,B_val,A_val,R_target,G_target,B_target,A_target); //East

	return;	
	


}

void process_file(int x_root, int y_root, int R_val, int G_val, int B_val, int A_val, int R_target, int G_target, int B_target, int A_target)
{
        if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
                abort_("[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA "
                       "(lacks the alpha channel)");

        if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
                abort_("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)",
                       PNG_COLOR_TYPE_RGBA, png_get_color_type(png_ptr, info_ptr));


	puts("Starting fill");
	//start iterating from the root node
	flood_fill(x_root,y_root,R_val,G_val,B_val,A_val,R_target,G_target,B_target,A_target); 

	puts("Done");
	

}


int main(int argc, char **argv)
{
        if (argc != 13) {
                printf("%s", "Usage: flood_fill_png <png_in> <x_root> <y_root> <R> <G> <B> <A> <R_target> <G_target> <B_target> <A_target> <png_out>"
				"\nReplaces color in an image using a flood fill algo from a root node\n"
				);
		exit(1);
	}
        read_png_file(argv[1]);
        process_file(atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atoi(argv[6]),atoi(argv[7]),atoi(argv[8]),atoi(argv[9]),atoi(argv[10]),atoi(argv[11]));
	puts("Writing png..");
        write_png_file(argv[12]);

        return 0;
}
