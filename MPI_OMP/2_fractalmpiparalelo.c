#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>

#define OUTFILE "out_julia_paralel.bmp"
#define MASTER_RANK 0

int compute_julia_pixel(int x, int y, int largura, int altura, float tint_bias, unsigned char *rgb);
void write_bmp_header(MPI_File f, int largura, int altura);

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int altura_total;
    int area=0, largura = 0, altura = 0, local_i= 0;
    MPI_File output_file;
    unsigned char *pixel_array, *rgb;

    if ((argc <= 1)|(atoi(argv[1])<1)){
        fprintf(stderr,"Entre 'N' como um inteiro positivo! \n");
        return -1; }
    altura_total = atoi(argv[1]);

    int num_of_process;
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_process);

    int current_process;
    MPI_Comm_rank(MPI_COMM_WORLD, &current_process);


    int chunk = altura_total / num_of_process;
    int offset_start = chunk * current_process;
    int offset_end = offset_start + chunk;

    altura = chunk; largura = 2*altura_total; area=altura*largura*3;

    //Allocate mem for the pixels array
    pixel_array= calloc(area, sizeof(unsigned char));
    rgb = calloc(3, sizeof(unsigned char));
    for (int i = offset_start; i < offset_end; i++)
        for (int j = 0; j < largura*3; j+=3){
            compute_julia_pixel(j/3, i, largura, altura_total, 1.0, rgb);
            pixel_array[local_i]= rgb[0]; local_i++;
            pixel_array[local_i]= rgb[1]; local_i++;
            pixel_array[local_i]= rgb[2]; local_i++;
        }
    //Release mem for the pixels array
    free(rgb);

    MPI_File_open(MPI_COMM_WORLD, OUTFILE,
                  MPI_MODE_CREATE | MPI_MODE_WRONLY,
                  MPI_INFO_NULL, &output_file);

     if(current_process == MASTER_RANK) write_bmp_header(output_file, largura, altura_total);

    //escreve o array no arquivo
    if(current_process == MASTER_RANK){
      MPI_File_write_all(output_file, pixel_array, area, MPI_CHAR, MPI_STATUS_IGNORE);
    } else {
      MPI_Offset offset = current_process * area;
      MPI_File_write_at_all(output_file, offset, pixel_array, area, MPI_CHAR, MPI_STATUS_IGNORE);
    }

    printf("Computando linhas de pixel %d até %d, para uma área total de %d\n", offset_start, offset_end - 1, area);

    MPI_File_close(&output_file);

    free(pixel_array);

    MPI_Finalize();
    return 0;
} /* fim-programa */

int compute_julia_pixel(int x, int y, int largura, int altura, float tint_bias, unsigned char *rgb) {
  // Check coordinates
  if ((x < 0) || (x >= largura) || (y < 0) || (y >= altura)) {
    fprintf(stderr,"Invalid (%d,%d) pixel coordinates in a %d x %d image\n", x, y, largura, altura);
    return -1;
  }
  // "Zoom in" to a pleasing view of the Julia set
  float X_MIN = -1.6, X_MAX = 1.6, Y_MIN = -0.9, Y_MAX = +0.9;
  float float_y = (Y_MAX - Y_MIN) * (float)y / altura + Y_MIN ;
  float float_x = (X_MAX - X_MIN) * (float)x / largura  + X_MIN ;
  // Point that defines the Julia set
  float julia_real = -.79;
  float julia_img = .15;
  // Maximum number of iteration
  int max_iter = 300;
  // Compute the complex series convergence
  float real=float_y, img=float_x;
  int num_iter = max_iter;
  while (( img * img + real * real < 2 * 2 ) && ( num_iter > 0 )) {
    float xtemp = img * img - real * real + julia_real;
    real = 2 * img * real + julia_img;
    img = xtemp;
    num_iter--;
  }

  // Paint pixel based on how many iterations were used, using some funky colors
  float color_bias = (float) num_iter / max_iter;
  rgb[0] = (num_iter == 0 ? 200 : - 500.0 * pow(tint_bias, 1.2) *  pow(color_bias, 1.6));
  rgb[1] = (num_iter == 0 ? 100 : -255.0 *  pow(color_bias, 0.3));
  rgb[2] = (num_iter == 0 ? 100 : 255 - 255.0 * pow(tint_bias, 1.2) * pow(color_bias, 3.0));

  return 0;
} /*fim compute julia pixel */

void write_bmp_header(MPI_File f, int largura, int altura) {

  unsigned int row_size_in_bytes = largura * 3 +
          ((largura * 3) % 4 == 0 ? 0 : (4 - (largura * 3) % 4));

  // Define all fields in the bmp header
  char id[2] = "BM";
  unsigned int filesize = 54 + (int)(row_size_in_bytes * altura * sizeof(char));
  short reserved[2] = {0,0};
  unsigned int offset = 54;

  unsigned int size = 40;
  unsigned short planes = 1;
  unsigned short bits = 24;
  unsigned int compression = 0;
  unsigned int image_size = largura * altura * 3 * sizeof(char);
  int x_res = 0;
  int y_res = 0;
  unsigned int ncolors = 0;
  unsigned int importantcolors = 0;

  // Write the bytes to the file, keeping track of the
  // number of written "objects"
  MPI_Request request;
  MPI_File_iwrite(f, id, 2, MPI_CHAR, &request);
  MPI_File_iwrite(f, &filesize, 1, MPI_INT, &request);
  MPI_File_iwrite(f, reserved, 2, MPI_SHORT, &request);
  MPI_File_iwrite(f, &offset, 1, MPI_INT, &request);
  MPI_File_iwrite(f, &size, 1, MPI_INT, &request);
  MPI_File_iwrite(f, &largura, 1, MPI_INT, &request);
  MPI_File_iwrite(f, &altura, 1, MPI_INT, &request);
  MPI_File_iwrite(f, &planes, 1, MPI_SHORT, &request);
  MPI_File_iwrite(f, &bits, 1, MPI_SHORT, &request);
  MPI_File_iwrite(f, &compression, 1, MPI_INT, &request);
  MPI_File_iwrite(f, &image_size, 1, MPI_INT, &request);
  MPI_File_iwrite(f, &x_res, 1, MPI_INT, &request);
  MPI_File_iwrite(f, &y_res, 1, MPI_INT, &request);
  MPI_File_iwrite(f, &ncolors, 1, MPI_INT, &request);
  MPI_File_iwrite(f, &importantcolors, 1, MPI_INT, &request);
  MPI_Wait(&request, MPI_STATUS_IGNORE);


  // Success means that we wrote 17 "objects" successfully
  // return (ret != 17);
} /* fim write bmp-header */