# O10 1D tactile: one uint8[] per region (1g per sample, max 255g). Sizes: thumb 16; index/middle/ring/little 18 each; palm 78; dorsum 102.
std_msgs/Header header
uint8[] thumb
uint8[] index
uint8[] middle
uint8[] ring
uint8[] little
uint8[] palm
uint8[] dorsum
