#include "key_binding_set.h"

#include "action_set.h"
#include "key_grab_set.h"
#include "string_util.h"
#include "application.h"
#include "backend.h"
#include "config.h"

using std::string;
using std::vector;

KeyBindingSet::KeyBindingSet(const char *config_group, const ActionSet *actions) :
    _config_group(config_group),
    _actions(actions),
    _key_grabs(Application::backend()->createKeyGrabSet())
{
}

KeyBindingSet::~KeyBindingSet()
{
    clear();
    delete _key_grabs;
    _key_grabs = 0;
}

void KeyBindingSet::clear()
{
    for (Binding &b : _bindings)
        delete b.parameters;
    _bindings.clear();
    _key_grabs->clear();
}

const KeyBindingSet::Binding *KeyBindingSet::find(int key_grab_id) const
{
    for (const Binding &binding : _bindings) {
        if (binding.key_grab_id == key_grab_id)
            return &binding;
    }
    return 0;
}

void KeyBindingSet::createBindings()
{
    clear();

    if (const Config::Section *section = Config::self()->section(_config_group)) {
        for (const Config::Entry &entry : section->entries()) {

            Binding binding;

            binding.key_grab_id = _key_grabs->addGrab(entry.key);

            if (binding.key_grab_id != INVALID_INDEX) {
                vector<string> tokens;
                StringUtil::tokenize(entry.value, ' ', tokens, false);

                if (!tokens.empty()) {
                    string &action_name = tokens.front();
                    binding.action = _actions->find(action_name);

                    if (binding.action) {
                        if (binding.action->type == Action::TYPE_COMPLEX) {
                            size_t num_parameters = tokens.size() - 1;
                            if (num_parameters) {
                                vector<string> params;
                                params.reserve(num_parameters);
                                for (size_t i = 1; i < tokens.size(); i++)
                                    params.push_back(tokens[i]);
                                binding.parameters = binding.action->complex_action->parseParameters(params);
                            }
                        }

                        _bindings.push_back(binding);
                    } else
                        debug<<"no such action:"<<action_name;
                } else
                    debug<<"missing action for key sequence "<<entry.key;
            }
        }
    } else
        debug<<"no such config section:"<<_config_group;
}
