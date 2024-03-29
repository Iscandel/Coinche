#ifndef H__RESOURCEIMAGE_010120142127__H
#define H__RESOURCEIMAGE_010120142127__H

#include "IResource.h"
#include "tools/Point.h"
#include "tools/Rectangle.h"
#include "ResourceTexture.h"

#include <boost/smart_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

class Image //ressource ?
{
public:
	Image(int textureId, const IntRect& rect, const Point<int>& offset = Point<int>());
	PtrResourceTexture getResourceTexture() {return myTexture;}
	const IntRect& getRect() const {return myRect;}
	const Point<int>& getOffset() const {return myOffset;}
	int getTextureId() {return myTextureId;}

	void free();
	void reload();
protected:
	IntRect myRect;
	Point<int> myOffset;
	PtrResourceTexture myTexture;
	int myTextureId;
}; //+ResourceImage manager qui contient des images d'un meme type

class ResourceImage : public IResource
{
public:
	ResourceImage(/*PtrImage Img*/);
	~ResourceImage(void);

	///////////////////////////////////////////////////////////////////////////
	// Renvoie un pointeur sur l'image num�ro num
	///////////////////////////////////////////////////////////////////////////
	PtrResourceTexture getResourceTexture(int num = 0) {return myImages[num].getResourceTexture();}

	Image& getImage(int num = 0) {return myImages[num];}

	///////////////////////////////////////////////////////////////////////////
	// Ajoute une image (Cas multi images)
	// Params : transp -> Couleur de transparence
	//			offset -> L'offset d'affichage
	//			offset2-> L'offset miroir
	//			img	   -> L'image
	///////////////////////////////////////////////////////////////////////////
	void add(const Image& im);

	///////////////////////////////////////////////////////////////////////////
	// Renvoie le nombre d'image contenue dans la ressource
	///////////////////////////////////////////////////////////////////////////
	unsigned int getNumberImages() const {return (unsigned int)myImages.size();}

	///////////////////////////////////////////////////////////////////////////
	// Renvoie l'offset de l'image numImage
	///////////////////////////////////////////////////////////////////////////
	const Point<int>& getOffset(int numImage = 0) const {return myImages[numImage].getOffset();}

	///////////////////////////////////////////////////////////////////////////
	// Renvoie l'offset miroir de l'image numImage
	///////////////////////////////////////////////////////////////////////////
	//const Point<int>& getOffset2(int numImage = 0)  const {return myImages[numImage].getOffset2();}

	///////////////////////////////////////////////////////////////////////////
	// Affecte les sons
	///////////////////////////////////////////////////////////////////////////
	void addSound(const std::vector<std::string>& vec)
	{
		mySounds = vec;
	}

	///////////////////////////////////////////////////////////////////////////
	// Renvoie l'id du son associ� � l'image
	///////////////////////////////////////////////////////////////////////////
	std::string getSound(int num = 0) const {return num < (int)mySounds.size() ? mySounds[num] : "";}

	///////////////////////////////////////////////////////////////////////////
	// Recharge le set d'images � l'aide du vecteur d'images pass� en param�tre
	// Si smooth vaut true, on applique le smoothing aux images
	///////////////////////////////////////////////////////////////////////////
	virtual void reload();

	///////////////////////////////////////////////////////////////////////////
	// Supprime les images de la ressource, mais conserve les offsets et la 
	// couleur de transparence
	///////////////////////////////////////////////////////////////////////////
	virtual void free();

protected:
	//int id; //ok ?
	std::vector<Image> myImages;
	//ResourceTexture dans Image ou dans dans ResourceImage ?

	std::vector<std::string> mySounds;
};

typedef boost::shared_ptr<ResourceImage> PtrResourceImage;

//class ResourceImage : public IResource
//{
//public:
//	ResourceImage(/*PtrImage Img*/);
//	~ResourceImage(void);
//
//	///////////////////////////////////////////////////////////////////////////
//	// Renvoie un pointeur sur l'image num�ro num
//	///////////////////////////////////////////////////////////////////////////
//	PtrImage getImage(int num = 0) {return MyImages[num].getImage();}
//
//	///////////////////////////////////////////////////////////////////////////
//	// Ajoute une image (Cas multi images)
//	// Params : transp -> Couleur de transparence
//	//			offset -> L'offset d'affichage
//	//			offset2-> L'offset miroir
//	//			img	   -> L'image
//	///////////////////////////////////////////////////////////////////////////
//	void add(const sf::Color& transp, const Position<int>& offset,
//						const Position<int>& offset2, PtrImage img = PtrImage());
//
//	///////////////////////////////////////////////////////////////////////////
//	//Ajoute une image standard (cas o� la ressource image ne contient qu'une image)
//	///////////////////////////////////////////////////////////////////////////
//	void add(PtrImage p, const Position<int>& offset = Position<int>(), const Position<int>& offset2 = Position<int>());
//
//	///////////////////////////////////////////////////////////////////////////
//	// Renvoie le nombre d'image contenue dans la ressource
//	///////////////////////////////////////////////////////////////////////////
//	unsigned int getNumberImages() const {return (unsigned int)MyImages.size();}
//
//	///////////////////////////////////////////////////////////////////////////
//	// Renvoie l'offset de l'image numImage
//	///////////////////////////////////////////////////////////////////////////
//	const Position<int>& getOffset(int numImage = 0) const {return MyImages[numImage].getOffset();}
//
//	///////////////////////////////////////////////////////////////////////////
//	// Renvoie l'offset miroir de l'image numImage
//	///////////////////////////////////////////////////////////////////////////
//	const Position<int>& getOffset2(int numImage = 0)  const {return MyImages[numImage].getOffset2();}
//
//	///////////////////////////////////////////////////////////////////////////
//	// Affecte les sons
//	///////////////////////////////////////////////////////////////////////////
//	void addSound(const std::vector<std::string>& vec)
//	{
//		mySound = vec;
//	}
//
//	///////////////////////////////////////////////////////////////////////////
//	// Renvoie l'id du son associ� � l'image
//	///////////////////////////////////////////////////////////////////////////
//	std::string getSound(int num = 0) const {return num < (int)mySounds.size() ? mySounds[num] : "";}
//
//	///////////////////////////////////////////////////////////////////////////
//	// Recharge le set d'images � l'aide du vecteur d'images pass� en param�tre
//	// Si smooth vaut true, on applique le smoothing aux images
//	///////////////////////////////////////////////////////////////////////////
//	virtual void reload(const std::vector<PtrImage>&, bool smooth = false);
//
//	///////////////////////////////////////////////////////////////////////////
//	// Supprime les images de la ressource, mais conserve les offsets et la 
//	// couleur de transparence
//	///////////////////////////////////////////////////////////////////////////
//	virtual void free();
//
//protected:
//	//int id; //ok ?
//	std::vector<DataImage<::PtrImage> > MyImages;
//
//	std::vector<std::string> mySounds;
//};
//
//typedef boost::shared_ptr<ResourceImage> PtrResourceImage;

#endif


//Une classe de ressources encapsulant 1 BasiqueRessourcesImage (texture cartes) et d�finissant une image (ex 3 de pique)