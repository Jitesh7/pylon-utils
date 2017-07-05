#include "basler_utils.h"
#include <iostream>

using namespace Pylon;
using namespace GenApi;

struct Whitespace {
    size_t space;
    Whitespace(size_t s=0): space(s) {}
};

std::ostream& operator<<(std::ostream& ostr, const Whitespace& ws) {
    for (size_t i=0; i<ws.space; ++i) {
        ostr << ' ';
    }
    return ostr;
}


const char* bool_yn(bool b) {
    return b ? "Yes" : "No";
}


void recurse_node(INode* node, int depth) {
   
    Whitespace hind(4*depth), bind(4*depth+2);


    EInterfaceType intf = node->GetPrincipalInterfaceType();
  
    if (!IsReadable(node) && intf != intfICategory) {
        return;
    }
  
    std::cout << hind << node->GetDisplayName() << " - "
              << node->GetDescription() << "\n";

    if (intf != intfICategory) {

        std::cout << "\n";
        std::cout << bind << "Type: " << type_str(intf) << "\n";
        std::cout << bind << "Readable: " << bool_yn(IsReadable(node)) << "\n";

        if (IsReadable(node)) {

            std::cout << bind << "Writable: " << bool_yn(IsWritable(node)) << "\n";
            std::cout << bind << "Key: " << node->GetName() << "\n";

            IValue* value = dynamic_cast<IValue*>(node);
            if (value) { 
                std::cout << bind << "Current value: " << value->ToString() << "\n";
            }


            if (intf == intfIInteger) {
                if (IsWritable(node)) {
                    IInteger* iint = dynamic_cast<IInteger*>(node);
                    std::cout << bind << "Min value: " << iint->GetMin() << "\n";
                    std::cout << bind << "Max value: " << iint->GetMax() << "\n";
                    std::cout << bind << "Increment: " << iint->GetInc() << "\n";
                }
            } else if (intf == intfIFloat) {
                IFloat* ifloat = dynamic_cast<IFloat*>(node);
                std::cout << bind << "Unit: " << ifloat->GetUnit() << "\n";
                if (IsWritable(node)) { 
                    std::cout << bind << "Min value: " << ifloat->GetMin() << "\n";
                    std::cout << bind << "Max value: " << ifloat->GetMax() << "\n";
                    if (ifloat->HasInc()) {
                        std::cout << bind << "Increment: " << ifloat->GetInc() << "\n";
                    }
                }
            } else if (intf == intfIEnumeration) {

                IEnumeration* ienumeration = dynamic_cast<IEnumeration*>(node);



                StringList_t symbolics;
                ienumeration->GetSymbolics(symbolics);

                NodeList_t entries;
                ienumeration->GetEntries(entries);
                std::cout << bind << "Entries:\n\n";
                for (NodeList_t::const_iterator j=entries.begin();
                     j!=entries.end(); ++j) {
                    
                    INode* node = *j;
                    IEnumEntry* entry = dynamic_cast<IEnumEntry*>(*j);
                    gcstring sym = entry->GetSymbolic();

                    bool found = false;
                    
                    for (StringList_t::const_iterator k=symbolics.begin();
                         k!=symbolics.end(); ++k) {
                        if (*k == sym) {
                            found = true;
                            break;
                        }
                    }
                    
                    if (found) {
                        std::cout << bind << "  " << sym << " - " << node->GetToolTip() << "\n";
                    }
                    
                }


        
            }

        }

    } 

    std::cout << "\n";

    NodeList_t children;

    node->GetChildren(children);

    for (NodeList_t::const_iterator i=children.begin();
         i!=children.end(); ++i) {

        INode* child = *i;
        if (child->IsFeature() && 
            IsVisible(child->GetVisibility(), Expert)) {
            recurse_node(child, depth+1);
        }

    }

  


}

int main(int argc, char** argv) {


    PylonInitialize();

    try {

        CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice());

        std::cout << "Using device " << camera.GetDeviceInfo().GetModelName() << std::endl;

        camera.Open();

        std::cout << "Opened device successfully.\n";

        // Get the camera control object.
        INodeMap &control = camera.GetNodeMap();

        INode* root = control.GetNode("Root");

        if (root) {
            recurse_node(root, 0);
        }

    } catch (const GenericException& e) {

        std::cerr << "error: " << e.GetDescription() << "\n";
        return 1;

    }

    return 0;

}
