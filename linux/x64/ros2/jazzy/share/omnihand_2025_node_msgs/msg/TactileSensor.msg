# OmniHand 2025 (O10) 1D tactile: one uint8[] per region; 1g per point, max 255g. 
std_msgs/Header header
uint8[] thumb   #  16 points
uint8[] index   #  18 points
uint8[] middle  #  18 points
uint8[] ring    #  18 points
uint8[] little  #  18 points
uint8[] palm    #  78 points
uint8[] dorsum  # 102 points
