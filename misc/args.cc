/*
 *  Copyright (C) 2000-2002  The Exult Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifndef ALPHA_LINUX_CXX
#  include <iostream>
#endif

#include "args.h"

using std::cerr;
using std::endl;
using std::string;
using std::strtol;
using std::strtoul;

void	Args::process(int argc,char **argv)
{
	for(int i=1;i<argc;i++)
	{
		for(unsigned int j=0;j<options.size() && i<argc;j++)
		{
			switch(options[j].valuetype)
			{
				case Option::no_type:
					continue;
				case Option::type_bool:
					if(options[j].option==argv[i])
						*(options[j]._bool_val)=options[j]._bool_default;
					break;
				case Option::type_str:
				{
					if(options[j].option==argv[i])
					{
						// We want the _next_ argument
						if(++i>=argc)
						{
							cerr << "Data not specified for argument '" << options[j].option <<"'. Using default." << endl;
							break;
						}
						*(options[j]._str_val)=argv[i];
					}
					break;
				}
				case Option::type_sint:
				{
					if(options[j].option==argv[i])
					{
						// We want the _next_ argument
						if(++i>=argc)
						{
							cerr << "Data not specified for argument '" << options[j].option <<"'. Using default." << endl;
							break;
						}
						*(options[j]._sint_val)=strtol(argv[i],0,10);
					}
					break;
				}
				case Option::type_uint:
				{
					if(options[j].option==argv[i])
					{
						// We want the _next_ argument
						if(++i>=argc)
						{
							cerr << "Data not specified for argument '" << options[j].option <<"'. Using default." << endl;
							break;
						}
						*(options[j]._uint_val)=strtoul(argv[i],0,10);
					}
					break;
				}
			}
		}
	}

}