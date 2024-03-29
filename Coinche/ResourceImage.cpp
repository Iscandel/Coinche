#include "ResourceImage.h"
#include "TextureManager.h"

///////////////////////////////////////////////////////////////////////////////
// Constructeur
///////////////////////////////////////////////////////////////////////////////
ResourceImage::ResourceImage()
{
	myLoadingStatus = IResource::NOT_LOADED;
}

///////////////////////////////////////////////////////////////////////////////
// Destructeur
///////////////////////////////////////////////////////////////////////////////
ResourceImage::~ResourceImage(void)
{
}

///////////////////////////////////////////////////////////////////////////////
// Ajoute une image (Cas multi images)
// Params : transp -> Couleur de transparence
//			offset -> L'offset d'affichage
//			offset2-> L'offset miroir
//			img	   -> L'image
// 
// NB : C'est au manager de fournir l'image. Peu importe si il r�cup�re le chemin
// dans RessourceImage ou dataImage ou sur lui. Le manager sait quel dat utiliser
// pour charger l'image.
///////////////////////////////////////////////////////////////////////////////
//void BasiqueResourceImage::add(const Color& transp, 
//							 const Position<int>& offset,
//							 const Position<int>& offset2,
//							 PtrImage img)
//{
//	//Ajout de la DataImage
//	MyImages.push_back(DataImage<PtrImage>(transp, offset, offset2));
//
//	//Si l'image est pr�sente, on l'ajoute aussi
//	if(img != NULL)
//	{
//		//On n'indique pas que la ressource est charg�e, il peut y avoir
//		//d'autres images
//		MyImages.back().reload(img);
//	}	
//
//	//On rappelle que la ressource n'est pas charg�e
//	myLoadingStatus = IResource::NOT_LOADED;
//}

Image::Image(int textureId, const IntRect& rect, const Point<int>& offset)
:myTextureId(textureId)
,myRect(rect)
,myOffset(offset)
{
}

void Image::reload()
{
	myTexture = TextureManager::getInstance()->getResource(myTextureId);
}

void Image::free()
{
	myTexture.reset();
}

///////////////////////////////////////////////////////////////////////////////
// Cas image unique
///////////////////////////////////////////////////////////////////////////////
void ResourceImage::add(const Image& im)
{
	myImages.push_back(im);
}

///////////////////////////////////////////////////////////////////////////////
// Lib�ration des images sans toucher aux offsets / couleur de transparence
///////////////////////////////////////////////////////////////////////////////
void ResourceImage::free()
{
	for(unsigned int i = 0; i < myImages.size(); i++)
	{
		myImages[i].free();
	}

	//On indique que la ressource n'est plus charg�e
	myLoadingStatus = IResource::NOT_LOADED;
}

///////////////////////////////////////////////////////////////////////////////
// Recharge les images � l'aide du vecteur d'images pass� en param�tre et 
// applique le smoothing si le bool�en smooth vaut true
///////////////////////////////////////////////////////////////////////////////
void ResourceImage::reload()
{
	myLoadingStatus = IResource::IS_LOADING;

	//On recharge toutes les images
	for(unsigned int i = 0; i < myImages.size(); i++)
	{
		myImages[i].reload();
	}

	//On indique que la ressource est charg�e
	myLoadingStatus = IResource::IS_LOADED;
}