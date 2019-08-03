#ifndef FW_CMSEXT_DECLARATIONS_H
#define FW_CMSEXT_DECLARATIONS_H

#include <cms/declarations.h>

/* NOTE !!!
  Class constructor must be called from [modules] directory. If i tried move it
  to [cmsext], static variable [static FW::Core::ApplicationBase::on_init_vec]
  after set callbacks was "cleared" on inicialization of
  [FW::Core::ApplicationBase]. I don't know why.
*/
#include <framework/core/application.h>
#include <framework/global.h>
#define CMS_BEGIN(module_name, module_args)                                  \
  namespace module_name {                                                    \
  struct CMSEXT_BIND_CLASS_##module_name {                                   \
    CMSEXT_BIND_CLASS_##module_name(void (*on_load)(void),                   \
                                    void (*on_init)(void),                   \
                                    void (*on_terminate)(void),              \
                                    void (*on_reload)(void),                 \
                                    const std::string& args) {               \
      if (args.find("try_run_first") != std::string::npos) {                 \
        FW::Core::ApplicationBase::on_load_vec.insert(                       \
            FW::Core::ApplicationBase::on_load_vec.begin(), on_load);        \
        FW::Core::ApplicationBase::on_init_vec.insert(                       \
            FW::Core::ApplicationBase::on_init_vec.begin(), on_init);        \
        FW::Core::ApplicationBase::on_terminate_vec.insert(                  \
            FW::Core::ApplicationBase::on_terminate_vec.begin(),             \
            on_terminate);                                                   \
        FW::Core::ApplicationBase::on_reload_vec.insert(                     \
            FW::Core::ApplicationBase::on_reload_vec.begin(), on_reload);    \
      } else {                                                               \
        FW::Core::ApplicationBase::on_load_vec.push_back(on_load);           \
        FW::Core::ApplicationBase::on_init_vec.push_back(on_init);           \
        FW::Core::ApplicationBase::on_terminate_vec.push_back(on_terminate); \
        FW::Core::ApplicationBase::on_reload_vec.push_back(on_reload);       \
      }                                                                      \
    }                                                                        \
  };                                                                         \
  void on_load();                                                            \
  void on_init();                                                            \
  void on_terminate();                                                       \
  void on_reload();                                                          \
  static inline CMSEXT_BIND_CLASS_##module_name module_name(                 \
      &on_load, &on_init, &on_terminate, &on_reload, module_args);
#define CMS_END() }
#endif  // #ifndef FW_CMSEXT_DECLARATIONS_H
