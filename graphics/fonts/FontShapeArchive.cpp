/*
Copyright (C) 2003-2005 The Pentagram team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "pent_include.h"

#include "FontShapeArchive.h"
#include "util.h"
#include "ShapeFont.h"
#include "ConfigFileManager.h"

DEFINE_RUNTIME_CLASSTYPE_CODE(FontShapeArchive,ShapeArchive);

ShapeFont* FontShapeArchive::getFont(uint32 fontnum)
{
	return p_dynamic_cast<ShapeFont*>(getShape(fontnum));
}

void FontShapeArchive::cache(uint32 shapenum)
{
	if (shapenum >= count) return;
	if (shapes.empty()) shapes.resize(count);

	if (shapes[shapenum]) return;

	uint32 shpsize;
	uint8 *data = getRawObject(shapenum, &shpsize);

	if (!data || shpsize == 0) return;

	// Auto detect format
	if (!format) format = Shape::DetectShapeFormat(data,shpsize);
	
	if (!format)
	{
		delete [] data;
		perr << "Error: Unable to detect shape format for flex." << std::endl;
		return;
	}

	Shape* shape = new ShapeFont(data, shpsize, format, id, shapenum);
	if (palette) shape->setPalette(palette);

	shapes[shapenum] = shape;
}

void FontShapeArchive::setHVLeads()
{
	ConfigFileManager* config = ConfigFileManager::get_instance();

	std::map<Pentagram::istring, std::string> leadkeyvals;

	leadkeyvals = config->listKeyValues("game/fontleads");
	std::map<Pentagram::istring, std::string>::iterator iter;
	for (iter = leadkeyvals.begin(); iter != leadkeyvals.end(); ++iter)
	{
		int fontnum = std::atoi(iter->first.c_str());
		std::string leaddesc = iter->second;

		std::vector<std::string> vals;
		Pentagram::SplitString(leaddesc, ',', vals);
		if (vals.size() != 2) {
			perr << "Invalid hlead/vlead description: " << leaddesc
				 << std::endl;
			continue;
		}

		int hlead = std::atoi(vals[0].c_str());
		int vlead = std::atoi(vals[1].c_str());

		ShapeFont* font = getFont(fontnum);
		if (font) {
			font->setHLead(hlead);
			font->setVLead(vlead);
		}
	}
}
