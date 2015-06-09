#include "key_binding_set.h"

#include "action_set.h"
#include "string_util.h"
#include "application.h"
#include "backend.h"
#include "abstract_key_sequence.h"
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
}

void KeyBindingSet::clear()
{
    for (Binding &b : _bindings) {
        Application::backend()->releaseKeyGrab(b.key_sequence);
        delete b.key_sequence;
        delete b.parameters;
    }
    _bindings.clear();
}

const KeyBindingSet::Binding *KeyBindingSet::find(const AbstractKeySequence *sequence) const {
    for (const Binding &binding : _bindings) {
        assert(binding.key_sequence);
        if (binding.key_sequence->equals(sequence))
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
            bool was_key_grabbed = false;
            bool was_added = false;

            binding.key_sequence = Application::backend()->parseKeySequence(entry.key);

            if (binding.key_sequence) {
                was_key_grabbed = Application::backend()->addKeyGrab(binding.key_sequence);
                if (was_key_grabbed) {
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
                            was_added = true;
                        } else
                            debug<<"no action"<<action_name<<"in set"<<_config_group;
                    } else
                        debug<<"missing action for key sequence "<<entry.key;
                }
            }

            if (!was_added) {
                if (was_key_grabbed)
                    Application::backend()->releaseKeyGrab(binding.key_sequence);
                delete binding.key_sequence;
                binding.key_sequence = 0;
                delete binding.parameters;
                binding.parameters = 0;
            }
        }
    } else
        debug<<"no such config section:"<<_config_group;
}
