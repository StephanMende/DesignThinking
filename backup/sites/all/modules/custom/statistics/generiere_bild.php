<?php
/*
// create image
$image = imagecreatetruecolor(600, 400);

// allocate some colors
$white = imagecolorallocate($image, 0xFF, 0xFF, 0xFF);
$gray = imagecolorallocate($image, 0xC0, 0xC0, 0xC0);
$darkgray = imagecolorallocate($image, 0x90, 0x90, 0x90);
$navy = imagecolorallocate($image, 0x90, 0x90, 0x90);
$navy = imagecolorallocate($image, 0x00, 0x00, 0x80);
$darknavy = imagecolorallocate($image, 0x00, 0x00, 0x50);
$red = imagecolorallocate($image, 0xFF, 0x00, 0x00);
$darkred = imagecolorallocate($image, 0x90, 0x00, 0x00);
$green = imagecolorallocate($image, 0x00, 0xFF, 0x00);
$darkgreen = imagecolorallocate($image, 0x0, 0x90, 0x00);

// make the 3D effect
for ($i = 220; $i > 200; $i--) {
    imagefilledarc($image, 300, $i, 500, 250, 0, 45, $darknavy, IMG_ARC_PIE);
    imagefilledarc($image, 300, $i, 500, 250, 45, 360, $darkgreen, IMG_ARC_PIE);
}

imagefilledarc($image, 300, 200, 500, 250, 0, 45, $navy, IMG_ARC_PIE);
imagefilledarc($image, 300, 200, 500, 250, 45, 360, $green, IMG_ARC_PIE);

// flush image
header('Content-type: image/png');
imagepng($image);
imagedestroy($image);
*/
?>