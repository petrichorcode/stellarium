/*
 * Stellarium
 * Copyright (C) 2002 Fabien Chereau
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _SOLARSYSTEM_HPP_
#define _SOLARSYSTEM_HPP_
//sun is already defined in Sun C/Solaris
#if defined(sun)
#undef sun
#endif
#include <vector>
#include <functional>

#include "StelObjectModule.hpp"
#include "STextureTypes.hpp"

class Orbit;
class LoadingBar;
class Translator;
class StelObject;
class Planet;
class SFont;
class StelCore;
class Projector;
class Navigator;
class QSettings;

//! @class SolarSystem
//! This StelObjectModule derivative is used to model SolarSystem boies.
//! This includes the Major Planets, Minor Planets and Comets.
class SolarSystem : public StelObjectModule
{
	Q_OBJECT;

public:
	SolarSystem();
	virtual ~SolarSystem();

	///////////////////////////////////////////////////////////////////////////
	// Methods defined in the StelModule class
	//! Initialize the SolarSystem.
	//! Includes:
	//! - loading planetary body orbital and model data from data/ssystem.ini
	//! - perform initial planet position calculation
	//! - set display options from application settings
	virtual void init();

	//! Draw SolarSystem objects (planets).
	//! @param core The StelCore object.
	//! @return The maximum squared distance in pixels that any SolarSystem object
	//! has travelled since the last update.
	virtual void draw(StelCore *core);
	
	//! Update time-varying components.
	//! This includes planet motion trails.
	virtual void update(double deltaTime);
	
	//! Translate names.
	virtual void updateI18n();
	
	//! Called when a new object is selected.
	virtual void selectedObjectChangeCallBack(StelModuleSelectAction action=StelModule::ReplaceSelection);
	
	//! Load a color scheme
	virtual void setStelStyle(const StelStyle& style);
	
	//! Used to determine what order to draw the various StelModules.
	virtual double getCallOrder(StelModuleActionName actionName) const;
	
	///////////////////////////////////////////////////////////////////////////
	// Methods defined in StelObjectManager class
	//! Search for SolarSystem objects in some area around a point.
	//! @param v A vector representing a point in the sky.
	//! @param limitFov The radius of the circle around the point v which
	//! defines the size of the area to search.
	//! @param core the core object
	//! @return A STL vector of SpelObjectP (pointers) containing all SolarSystem
	//! objects found in the specified area. This vector is not sorted by distance 
	//! from v.
	virtual vector<StelObjectP> searchAround(const Vec3d& v, double limitFov, const StelCore* core) const;
	
	//! Search for a SolarSystem object based on the localised name.
	//! @param NameI18n The case in-sensistive translated planet name.
	//! @return A StelObjectP for the object if found, else NULL.
	virtual StelObjectP searchByNameI18n(const QString& nameI18n) const;

	//! Search for a SolarSystem object based on the English name.
	//! @param NameI18n The case in-sensistive translated planet name.
	//! @return A StelObjectP for the object if found, else NULL.
	virtual StelObjectP searchByName(const QString& name) const;
	
	//! Find objects by translated name prefix.
	//! Find and return the list of at most maxNbItem objects auto-completing 
	//! the passed object I18n name.
	//! @param objPrefix the case insensitive first letters of the searched object.
	//! @param maxNbItem the maximum number of returned object names.
	//! @return a list of matching object name by order of relevance, or an empty list if nothing matches.
	virtual QStringList listMatchingObjectsI18n(const QString& objPrefix, int maxNbItem=5) const;

public slots:
	///////////////////////////////////////////////////////////////////////////
	// Method callable from script and GUI
	// Properties setters and getters
	//! Set flag which determines if planets are drawn or hidden.
	void setFlagPlanets(bool b);
	//! Get the current value of the flag which determines if planet are drawn or hidden.
	bool getFlagPlanets() const;

	//! Set flag which determines if planet trails are drawn or hidden.
	void setFlagTrails(bool b);
	//! Get the current value of the flag which determines if planet trails are drawn or hidden.
	bool getFlagTrails() const;
	
	//! Set flag which determines if planet hints are drawn or hidden along labels
	void setFlagHints(bool b);
	//! Get the current value of the flag which determines if planet hints are drawn or hidden along labels
	bool getFlagHints() const;	
	
	//! Set flag which determines if planet labels are drawn or hidden.
	void setFlagLabels(bool b);
	//! Get the current value of the flag which determines if planet labels are drawn or hidden.
	bool getFlagLabels() const;
	
	//! Set the amount of planet labels. The real amount is also proportional with FOV.
	//! The limit is set in function of the planets magnitude
	//! @param a the amount between 0 and 10. 0 is no labels, 10 is maximum of labels
	void setLabelsAmount(float a) {labelsAmount=a;}
	//! Get the amount of planet labels. The real amount is also proportional with FOV.
	//! @return the amount between 0 and 10. 0 is no labels, 10 is maximum of labels
	float getLabelsAmount(void) const {return labelsAmount;}
	
	//! Set flag which determines if planet orbits are drawn or hidden.
	void setFlagOrbits(bool b);
	//! Get the current value of the flag which determines if planet orbits are drawn or hidden.
	bool getFlagOrbits() const {return flagOrbits;}

	//! Set flag which determines if the light travel time calculation is used or not.
	void setFlagLightTravelTime(bool b) {flagLightTravelTime = b;}
	//! Get the current value of the flag which determines if light travel time 
	//! calculation is used or not.
	bool getFlagLightTravelTime(void) const {return flagLightTravelTime;}	
	
	//! Set planet names font size.
	void setFontSize(float newFontSize);
	
	//! Set the color used to draw planet labels.
	void setLabelsColor(const Vec3f& c);
	//! Get the current color used to draw planet labels.
	const Vec3f& getLabelsColor(void) const;
	
	//! Set the color used to draw planet orbit lines.
	void setOrbitsColor(const Vec3f& c);
	//! Get the current color used to draw planet orbit lines.
	Vec3f getOrbitsColor(void) const;
	
	//! Set the color used to draw planet trails.
	void setTrailsColor(const Vec3f& c);
	//! Get the current color used to draw planet trails.
	Vec3f getTrailsColor(void) const;
	
	//! Set flag which determines if Earth's moon is scaled or not.
	void setFlagMoonScale(bool b);
	//! Get the current value of the flag which determines if Earth's moon is scaled or not.
	bool getFlagMoonScale(void) const {return flagMoonScale;}
	
	//! Set the display scaling factor for Earth's moon.
	void setMoonScale(float f);
	//! Get the display scaling factor for Earth's oon.
	float getMoonScale(void) const {return moonScale;}		

public:
	///////////////////////////////////////////////////////////////////////////
	// Other public methods
	//! Get a pointer to a Planet object.
	//! @param planetEnglishName the English name of the desired planet.
	//! @return The matching planet pointer if exists or NULL.
	Planet* searchByEnglishName(QString planetEnglishName) const;
	
	//! Get the Planet object pointer for the Sun.
	Planet* getSun(void) const {return sun;}
	
	//! Get the Planet object pointer for the Earth.
	Planet* getEarth(void) const {return earth;}
	
	//! Get the Planet object pointer for Earth's moon.
	Planet* getMoon(void) const {return moon;}
	
	//! Determine if a lunar eclipse is close at hand?
	bool nearLunarEclipse();
	
	///////////////////////////////////////////////////////////////////////////////////////
	// DEPRECATED
	///////////////////////////////////////////////////////////////////////////////////////
	//! Get a hash of locale and ssystem.ini names for use with the TUI.
	//! @return A newline delimited hash of localized:standard planet names.
	//! Planet translated name is PARENT : NAME
	QString getPlanetHashString();
 
	//! Compute the position and transform matrix for every element of the solar system.
	//! @param observerPos Position of the observer in heliocentric ecliptic frame. 
	//! (Required for light travel time computation.)
	void computePositions(double date, const Vec3d& observerPos = Vec3d(0,0,0));
	
	//! Get the list of all the bodies of the solar system.
	const vector<Planet*>& getAllPlanets() const {return systemPlanets;}
	
private:
	//! Search for SolarSystem objects which are close to the position given 
	//! in earth equatorial position.
	//! @param v A position in earth equatorial position.
	//! @param core the StelCore object.
	//! @return a pointer to a StelObject if found, else NULL
	StelObject* search(Vec3d v, const StelCore* core) const;
	
	//! Update the planet motion trails.
	// void updateTrails(const Navigator* nav);

	//! Start/stop accumulating new trail data (clear old data).
	void startTrails(bool b);
	
	//! Compute the transformation matrix for every elements of the solar system.
	//! observerPos is needed for light travel time computation.
	void computeTransMatrices(double date, const Vec3d& observerPos = Vec3d(0,0,0));

	//! Draw a nice animated pointer around the object.
	void drawPointer(const StelCore* core);

	//! Load planet data from a file.
	void loadPlanets();

	Planet* sun;
	Planet* moon;
	Planet* earth;
	
	//! Set selected planets by englishName.
	//! @param englishName The planet name or "" to select no planet
	void setSelected(const QString& englishName);
	//! Set selected object from its pointer.
	void setSelected(StelObject* obj);	
	//! Get selected object's pointer.
	StelObject* getSelected(void) const {return selected;}	
	//! The currently selected planet.
	StelObject* selected;

	bool flagMoonScale;
	float moonScale;                 // Moon scale value

	double fontSize;
	SFont& planetNameFont;
	
	//! The amount of planets labels (between 0 and 10)
	float labelsAmount;
	
	vector<Planet*> systemPlanets;  // Vector containing all the bodies of the system
	
	// draw earth shadow on moon for lunar eclipses
	void drawEarthShadow(const Navigator * nav, Projector * prj);  

	// And sort them from the furthest to the closest to the observer
	struct biggerDistance : public binary_function<Planet*, Planet*, bool>
	{
		bool operator()(Planet* p1, Planet* p2);
	};

	STextureSP texEarthShadow;     // for lunar eclipses

	// Master settings
	bool flagOrbits;
	bool flagLightTravelTime;
	
	STextureSP texPointer;           // The selection pointer texture
	
	bool flagShow;
	
	//////////////////////////////////////////////////////////////////////////////////
	// DEPRECATED
	//////////////////////////////////////////////////////////////////////////////////
	const Planet* lastHomePlanet;    // for tracking home planet changes for trails
	vector<Orbit*> orbits;           // Pointers on created elliptical orbits
};


#endif // _SOLARSYSTEM_HPP_
