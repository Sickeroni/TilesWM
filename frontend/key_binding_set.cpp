#include "key_binding_set.h"
#include "string_util.h"

using std::string;
using std::vector;

KeyBindingSet::KeyBindingSet(const char *config_group, const ActionSet *actions) :
    _config_group(config_group),
    _actions(actions)
{
}

const KeyBindingSet::Binding *KeyBindingSet::find(int key_grab_id) const
{
    UNIMPLEMENTED
    return 0;
}

void KeyBindingSet::createBindings()
{
    UNIMPLEMENTED
#if 0
    if (const ConfigGroup *grp = Config::group(_config_group)) {
        for (size_t i = 0; i < grp->numEntries(); i++) {
            const Config::Entry &entry = grp->entry(i);

            Binding binding;

            binding.key_grab_id = _key_grabs->addGrab(entry.key);
            if (binding.key_grab_id != INVALID_INDEX) {
//                 const Action *action = parseAction(entry.value);
                vector<string> tokens;
                StringUtil::tokenize(entry.value, ' ', tokens, false);

                if (!tokens.empty()) {
                    string &action_name = tokens.front();
                    if (binding.action = _actions->find(action_name)) {
                        if (binding.action->type == Action::TYPE_COMPLEX) {
                            size_t num_parameters = tokens.size() - 1;
                            if (num_parameters) {
                                vector<String&> params;
                                params.reserve(num_parameters);
                                for (size_t i = 1; i < tokens.size(); i++)
                                    params.push_back(tokens[i]);
                                binding.parameters = binding.action->complex_action->parseParameters(params);
                            }
                        }
                    }
                }
            }
        }
    } else
        debug<<"no such config section:"<<_config_group;
#endif
}
