#ifndef DEVCPPLOADER_H
#define DEVCPPLOADER_H

#include "ibaseloader.h"

// forward decls
class caProject;

class DevCppLoader : public IBaseLoader
{
	public:
		DevCppLoader(caProject* project);
		virtual ~DevCppLoader();

		bool Open(const wxString& filename);
		bool Save(const wxString& filename);
	protected:
        caProject* m_pProject;
	private:
        DevCppLoader(){} // no default ctor
};

#endif // DEVCPPLOADER_H

