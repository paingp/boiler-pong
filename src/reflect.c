/* GIMP RGB C-Source image dump (reflect.c) */

const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[40 * 40 * 2 + 1];
} reflect = {
  40, 40, 2,
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340"
  "\001\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340"
  "\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340"
  "\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340"
  "\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\004"
  "r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340"
  "\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r"
  "\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340"
  "\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r"
  "\004r\004r\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340"
  "\001\340\001\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340"
  "\371\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\340\001\340"
  "\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\340\371\340\371\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\371\340\371\340\371\302"
  "y\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340"
  "\371\340\371\340\371\340\371\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001"
  "\340\001\340\001\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\340\371\340\371\340\371\340\371\340\371\340\371\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\340\001\340\001\004r\004r\004r\004r\004r"
  "\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\371\340\371\340\371\340\371\340\371\340"
  "\371\340\371\340\371\340\371\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340"
  "\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\371\340\371"
  "\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371"
  "\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r"
  "\004r\004r\004r\004r\004r\004r\340\371\340\371\340\371\340\371\340\371\340\371\340\371"
  "\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340"
  "\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340"
  "\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\371"
  "\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371"
  "\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\371\340\371\340"
  "\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340\371\340"
  "\371\340\371\340\371\340\371\340\371\340\371\340\371\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\371\340\371\340\371\340\371"
  "\004r\004r\004r\004r\004r\004r\004r\340\371\340\371\340\371\340\371\340\371\340\371\340"
  "\371\004r\004r\004r\004r\004r\004r\302y\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r"
  "\340\371\340\371\340\371\004r\004r\004r\004r\004r\004r\004r\004r\340\371\340\371\340\371"
  "\340\371\340\371\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\340\371\340\371\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\371\340\371"
  "\340\371\340\371\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\000\375\004r\340\371\340\371\302"
  "y\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\004r\004r\000\375\040\375\040\375\004r\340\371\302y\302y\302"
  "y\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\000\375\040\375\040\375\040\375\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\040\375\040\375\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r"
  "\004r\004r\004r\004r\004r\004r\000\375\040\375\040\375\040\375\000\375\302y\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\040\375\040\375\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r"
  "\004r\004r\004r\004r\004r\004r\000\375\040\375\040\375\040\375\040\375\000\375\302y\004r\004r\004r"
  "\004r\004r\004r\000\375\000\375\040\375\040\375\040\375\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r"
  "\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\000\375\040\375\040\375\040\375\040\375\040\375"
  "\000\375\000\375\000\375\040\375\040\375\040\375\040\375\040\375\040\375\000\375\000\375"
  "\040\375\040\375\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\000\375\000\375\040\375\040\375\040\375\040\375\040\375\040\375\000\375\040\375\040\375"
  "\040\375\040\375\040\375\040\375\040\375\040\375\040\375\000\375\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\000\375\040\375\040\375\040\375"
  "\040\375\040\375\040\375\040\375\040\375\040\375\040\375\040\375\040\375\040\375\040"
  "\375\040\375\040\375\000\375\000\375\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001"
  "\004r\004r\004r\004r\004r\004r\004r\004r\004r\000\375\000\375\000\375\040\375\040\375\040\375\000\375"
  "\040\375\040\375\000\375\040\375\040\375\040\375\040\375\040\375\040\375\004r\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\000\375\000\375\000\375\040\375\040\375\040\375\040\375\040\375\000\375\040\375\040"
  "\375\040\375\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340"
  "\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\000\375\040\375\040\375\040\375\040"
  "\375\040\375\000\375\000\375\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\340\001\340\001\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r"
  "\040\375\040\375\040\375\040\375\000\375\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\340\001\340\001\340\001\340\001\340\001\340\001\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\000\375\040\375\040\375\000\375\004r\004r\004r\004r\004r\004r\004r\004r"
  "\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\340\001\340\001\340\001\340"
  "\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\000\375\040\375\000\375\004r\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\000"
  "\375\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\340"
  "\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340"
  "\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\004r\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340"
  "\001\340\001\340\001\340\001\340\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r"
  "\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340"
  "\001\340\001\340\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004"
  "r\004r\004r\004r\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340"
  "\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340"
  "\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340"
  "\001\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\004r\340\001\340\001\340\001\340\001"
  "\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001\340\001",
};
