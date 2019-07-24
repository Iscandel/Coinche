//#include "DataImage.h"
//
/////////////////////////////////////////////////////////////////////////////////
//// Constructeur par d�faut
/////////////////////////////////////////////////////////////////////////////////
//DataImage::DataImage(void)
//{
//}
//
/////////////////////////////////////////////////////////////////////////////////
//// Premier constructeur
/////////////////////////////////////////////////////////////////////////////////
//DataImage::DataImage(const sf::Color& transp, const Coordonnees& offset, const Coordonnees& offset2)
//:monATransparence(true)
//,MonOffset(offset)
//,MonOffset2(offset2)
//,MaCouleurTransparence(transp)
//{
//}
//
/////////////////////////////////////////////////////////////////////////////////
//// Deuxi�me constructeur
/////////////////////////////////////////////////////////////////////////////////
//DataImage::DataImage(PtrImage MonImg, const Coordonnees& offset, const Coordonnees& offset2,  bool smooth)
//:monATransparence(false)
//,MonOffset(offset)
//,MonOffset2(offset2)
//,MonImage(MonImg)
//{
//	MonImage->SetSmooth(smooth);
//}
//
/////////////////////////////////////////////////////////////////////////////////
//// 3�me constructeur
/////////////////////////////////////////////////////////////////////////////////
//DataImage::DataImage(const Coordonnees& offset, const Coordonnees& offset2)
//:monATransparence(false)
//,MonOffset(offset)
//,MonOffset2(offset2)
//{
//}
//
/////////////////////////////////////////////////////////////////////////////////
//// Destructeur
/////////////////////////////////////////////////////////////////////////////////
//DataImage::~DataImage(void)
//{
//}
//
/////////////////////////////////////////////////////////////////////////////////
//// D�truit l'image
/////////////////////////////////////////////////////////////////////////////////
//void DataImage::libererImage()
//{
//	//r�initialisation du pointeur intelligent 
//	//(-> appel � delete si use_count == 1)
//	MonImage.reset();
//}
//
/////////////////////////////////////////////////////////////////////////////////
//// Recharge l'image avec l'image pass�e en param�tre
/////////////////////////////////////////////////////////////////////////////////
//void DataImage::recharger(PtrImage MonImg, bool smooth)
//{
//	//static int cpt = 0;
//	//cpt++;
//	//std::cout << cpt << std::endl;
//	MonImage = MonImg;
//
//	//Si on a besoin de mettre de la transparence pour une couleur
//	if(monATransparence)
//		MonImage->CreateMaskFromColor(MaCouleurTransparence);
//
//	MonImage->SetSmooth(smooth);
//}
//
