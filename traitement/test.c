#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
    // Charger l'image
    cv::Mat image = cv::imread(argv[1]);

    // Convertir l'image en niveaux de gris
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
 
    // Détecter les bords de l'image
    cv::Mat edges;
    cv::Canny(gray, edges, 50, 150);
 
    // Afficher les contours en blanc sur l'image d'origine
    cv::Mat output;
    image.copyTo(output);
    output.setTo(cv::Scalar(255,255,255), edges);
   
    // Afficher l'image
    cv::imshow("Contours", output);
    cv::waitKey(0);

    return 0;
}
