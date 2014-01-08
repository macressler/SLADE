
#include "Main.h"
#include "ThingType.h"
#include "Parser.h"
#include "GameConfiguration.h"

ThingType::ThingType(string name)
{
	// Init variables
	this->name = name;
	this->angled = true;
	this->hanging = false;
	this->shrink = false;
	this->colour = rgba_t(170, 170, 180, 255, 0);
	this->radius = 20;
	this->height = -1;
	this->scaleX = 1.0;
	this->scaleY = 1.0;
	this->fullbright = false;
	this->decoration = false;
	this->decorate = false;
	this->solid = false;
	this->zeth = -1;
	this->nexttype = 0;
	this->nextargs = 0;
	this->flags = 0;
	this->tagged = 0;

	// Init args
	args[0].name = "Arg1";
	args[1].name = "Arg2";
	args[2].name = "Arg3";
	args[3].name = "Arg4";
	args[4].name = "Arg5";
}

void ThingType::copy(ThingType* copy)
{
	// Check TT was given
	if (!copy) return;

	// Copy properties
	this->name = copy->name;
	this->group = copy->group;
	this->colour = copy->colour;
	this->radius = copy->radius;
	this->height = copy->height;
	this->scaleX = copy->scaleX;
	this->scaleY = copy->scaleY;
	this->angled = copy->angled;
	this->hanging = copy->hanging;
	this->fullbright = copy->fullbright;
	this->shrink = copy->shrink;
	this->sprite = copy->sprite;
	this->icon = copy->icon;
	this->translation = copy->translation;
	this->palette = copy->palette;
	this->decoration = copy->decoration;
	this->solid = copy->solid;
	this->zeth = copy->zeth;
	this->nexttype = copy->nexttype;
	this->nextargs = copy->nextargs;
	this->flags = copy->flags;
	this->tagged = copy->tagged;

	// Copy args
	for (unsigned a = 0; a < 5; a++)
		this->args[a] = copy->args[a];
}

string ThingType::getArgsString(int args[5])
{
	string ret;

	// Add each arg to the string
	for (unsigned a = 0; a < 5; a++)
	{
		// Skip if the arg name is undefined and the arg value is 0
		if (args[a] == 0 && this->args[a].name.StartsWith("Arg"))
			continue;

		ret += this->args[a].name;
		ret += ": ";
		ret += this->args[a].valueString(args[a]);
		ret += ", ";
	}

	// Cut ending ", "
	if (!ret.IsEmpty())
		ret.RemoveLast(2);

	return ret;
}

void ThingType::reset()
{
	// Reset variables
	this->name = "Unknown";
	this->group = "";
	this->sprite = "";
	this->icon = "";
	this->translation = "";
	this->palette = "";
	this->angled = true;
	this->hanging = false;
	this->shrink = false;
	this->colour = COL_WHITE;
	this->radius = 20;
	this->height = -1;
	this->scaleX = 1.0;
	this->scaleY = 1.0;
	this->fullbright = false;
	this->decoration = false;
	this->solid = false;
	this->zeth = -1;
	this->nexttype = 0;
	this->nextargs = 0;
	this->flags = 0;
	this->tagged = 0;

	// Reset args
	for (unsigned a = 0; a < 5; a++)
	{
		args[a].name = S_FMT("Arg%d", a+1);
		args[a].type = ARGT_NUMBER;
		args[a].custom_flags.clear();
		args[a].custom_values.clear();
	}
}

void ThingType::parse(ParseTreeNode* node)
{
	// Go through all child nodes/values
	ParseTreeNode* child = NULL;
	for (unsigned a = 0; a < node->nChildren(); a++)
	{
		child = (ParseTreeNode*)node->getChild(a);
		string name = child->getName();
		int arg = -1;

		// Name
		if (S_CMPNOCASE(name, "name"))
			this->name = child->getStringValue();

		// Args
		else if (S_CMPNOCASE(name, "arg1"))
			arg = 0;
		else if (S_CMPNOCASE(name, "arg2"))
			arg = 1;
		else if (S_CMPNOCASE(name, "arg3"))
			arg = 2;
		else if (S_CMPNOCASE(name, "arg4"))
			arg = 3;
		else if (S_CMPNOCASE(name, "arg5"))
			arg = 4;

		// Sprite
		else if (S_CMPNOCASE(name, "sprite"))
			this->sprite = child->getStringValue();

		// Icon
		else if (S_CMPNOCASE(name, "icon"))
			this->icon = child->getStringValue();

		// Radius
		else if (S_CMPNOCASE(name, "radius"))
			this->radius = child->getIntValue();

		// Height
		else if (S_CMPNOCASE(name, "height"))
			this->height = child->getIntValue();

		// Scale
		else if (S_CMPNOCASE(name, "scale"))
			this->scaleX = this->scaleY = child->getFloatValue();

		// ScaleX
		else if (S_CMPNOCASE(name, "scalex"))
			this->scaleX = child->getFloatValue();

		// ScaleY
		else if (S_CMPNOCASE(name, "scaley"))
			this->scaleY = child->getFloatValue();

		// Colour
		else if (S_CMPNOCASE(name, "colour"))
			this->colour.set(child->getIntValue(0), child->getIntValue(1), child->getIntValue(2));

		// Show angle
		else if (S_CMPNOCASE(name, "angle"))
			this->angled = child->getBoolValue();

		// Hanging object
		else if (S_CMPNOCASE(name, "hanging"))
			this->hanging = child->getBoolValue();

		// Shrink on zoom
		else if (S_CMPNOCASE(name, "shrink"))
			this->shrink = child->getBoolValue();

		// Fullbright
		else if (S_CMPNOCASE(name, "fullbright"))
			this->fullbright = child->getBoolValue();

		// Decoration
		else if (S_CMPNOCASE(name, "decoration"))
			this->decoration = child->getBoolValue();

		// Solid
		else if (S_CMPNOCASE(name, "solid"))
			this->solid = child->getBoolValue();

		// Translation
		else if (S_CMPNOCASE(name, "translation"))
		{
			this->translation += "\"";
			size_t v = 0;
			do
			{
				this->translation += child->getStringValue(v++);
			}
			while ((v < child->nValues()) && ((this->translation += "\", \""), true));
			this->translation += "\"";
		}

		// Palette override
		else if (S_CMPNOCASE(name, "palette"))
			this->palette = child->getStringValue();

		// Zeth icon
		else if (S_CMPNOCASE(name, "zeth"))
			this->zeth = child->getIntValue();

		// Pathed things stuff
		else if (S_CMPNOCASE(name, "nexttype"))
		{
			this->nexttype = child->getIntValue();
			this->flags |= THING_PATHED;
		}
		else if (S_CMPNOCASE(name, "nextargs"))
		{
			this->nextargs = child->getIntValue();
			this->flags |= THING_PATHED;
		}

		// Hexen's critters are weird
		else if (S_CMPNOCASE(name, "dragon"))
			this->flags |= THING_DRAGON;
		else if (S_CMPNOCASE(name, "script"))
			this->flags |= THING_SCRIPT;

		// Some things tag other things directly
		else if (S_CMPNOCASE(name, "tagged"))
		{
			string str = child->getStringValue();
			if (S_CMPNOCASE(str, "no")) this->tagged = AS_TT_NO;
			else if (S_CMPNOCASE(str, "sector")) this->tagged = AS_TT_SECTOR;
			else if (S_CMPNOCASE(str, "line")) this->tagged = AS_TT_LINE;
			else if (S_CMPNOCASE(str, "lineid")) this->tagged = AS_TT_LINEID;
			else if (S_CMPNOCASE(str, "lineid_hi5")) this->tagged = AS_TT_LINEID_HI5;
			else if (S_CMPNOCASE(str, "thing")) this->tagged = AS_TT_THING;
			else if (S_CMPNOCASE(str, "sector_back")) this->tagged = AS_TT_SECTOR_BACK;
			else if (S_CMPNOCASE(str, "sector_or_back")) this->tagged = AS_TT_SECTOR_OR_BACK;
			else if (S_CMPNOCASE(str, "sector_and_back")) this->tagged = AS_TT_SECTOR_AND_BACK;
			else if (S_CMPNOCASE(str, "line_negative")) this->tagged = AS_TT_LINE_NEGATIVE;
			else if (S_CMPNOCASE(str, "ex_1thing_2sector")) this->tagged = AS_TT_1THING_2SECTOR;
			else if (S_CMPNOCASE(str, "ex_1thing_3sector")) this->tagged = AS_TT_1THING_3SECTOR;
			else if (S_CMPNOCASE(str, "ex_1thing_2thing")) this->tagged = AS_TT_1THING_2THING;
			else if (S_CMPNOCASE(str, "ex_1thing_4thing")) this->tagged = AS_TT_1THING_4THING;
			else if (S_CMPNOCASE(str, "ex_1thing_2thing_3thing")) this->tagged = AS_TT_1THING_2THING_3THING;
			else if (S_CMPNOCASE(str, "ex_1sector_2thing_3thing_5thing")) this->tagged = AS_TT_1SECTOR_2THING_3THING_5THING;
			else if (S_CMPNOCASE(str, "ex_1lineid_2line")) this->tagged = AS_TT_1LINEID_2LINE;
			else if (S_CMPNOCASE(str, "ex_4thing")) this->tagged = AS_TT_4THING;
			else if (S_CMPNOCASE(str, "ex_5thing")) this->tagged = AS_TT_5THING;
			else if (S_CMPNOCASE(str, "ex_1line_2sector")) this->tagged = AS_TT_1LINE_2SECTOR;
			else if (S_CMPNOCASE(str, "ex_1sector_2sector")) this->tagged = AS_TT_1SECTOR_2SECTOR;
			else if (S_CMPNOCASE(str, "ex_1sector_2sector_3sector_4_sector")) this->tagged = AS_TT_1SECTOR_2SECTOR_3SECTOR_4SECTOR;
			else if (S_CMPNOCASE(str, "ex_sector_2is3_line")) this->tagged = AS_TT_SECTOR_2IS3_LINE;
			else if (S_CMPNOCASE(str, "ex_1sector_2thing")) this->tagged = AS_TT_1SECTOR_2THING;
			else
				this->tagged = child->getIntValue();
		}

		// Parse arg definition if it was one
		if (arg >= 0)
		{
			// Check for simple definition
			if (child->isLeaf())
			{
				// Set name
				args[arg].name = child->getStringValue();

				// Set description (if specified)
				if (child->nValues() > 1) args[arg].desc = child->getStringValue(1);
			}
			else
			{
				// Extended arg definition

				// Name
				ParseTreeNode* val = (ParseTreeNode*)child->getChild("name");
				if (val) args[arg].name = val->getStringValue();

				// Description
				val = (ParseTreeNode*)child->getChild("desc");
				if (val) args[arg].desc = val->getStringValue();

				// Type
				val = (ParseTreeNode*)child->getChild("type");
				string atype;
				if (val) atype = val->getStringValue();
				if (S_CMPNOCASE(atype, "yesno"))
					args[arg].type = ARGT_YESNO;
				else if (S_CMPNOCASE(atype, "noyes"))
					args[arg].type = ARGT_NOYES;
				else if (S_CMPNOCASE(atype, "angle"))
					args[arg].type = ARGT_ANGLE;
				else
					args[arg].type = ARGT_NUMBER;
			}
		}
	}
}

string ThingType::stringDesc()
{
	// Init return string
	string ret = S_FMT("\"%s\" in group \"%s\", colour %d,%d,%d, radius %d", CHR(name), CHR(group), colour.r, colour.g, colour.b, radius);

	// Add any extra info
	if (!sprite.IsEmpty()) ret += S_FMT(", sprite \"%s\"", CHR(sprite));
	if (!angled) ret += ", angle hidden";
	if (hanging) ret += ", hanging";
	if (fullbright) ret += ", fullbright";
	if (decoration) ret += ", decoration";
	if (decorate) ret += ", defined in DECORATE";

	return ret;
}
