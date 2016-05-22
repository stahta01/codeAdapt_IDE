#ifndef CLASSWIZARD_H
#define CLASSWIZARD_H

#include "api/plugin.h"

class ClassWizard : public caToolPlugin
{
    public:
        ClassWizard();
        ~ClassWizard();

        virtual void OnAttach();
        virtual void OnRelease(bool appShutDown);
        virtual int Configure(){ return -1; }
        virtual int Execute();
};

#endif // CLASSWIZARD_H
