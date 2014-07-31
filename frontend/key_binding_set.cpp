#include "key_binding_set.h"

#include "action_set.h"
#include "key_grab_set.h"
#include "string_util.h"
#include "application.h"
#include "backend.h"
#include "config.h"

using std::string;
using std::vector;

KeyBindingSet::KeyBindingSet(const std::string &name, const ActionSet *actions) :
    _config_group(std::string("keybindings.") + name),
    _actions(actions)
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
    delete _key_grabs;
    _key_grabs = 0;
}

const KeyBindingSet::Binding *KeyBindingSet::find(size_t key_grab_id) const
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

    assert(!_key_grabs);

    _key_grabs = Application::backend()->createKeyGrabSet();

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
                        debug<<"no action"<<action_name<<"in set"<<_config_group;
                } else
                    debug<<"missing action for key sequence "<<entry.key;
            }
        }
    } else
        debug<<"no such config section:"<<_config_group;
}
