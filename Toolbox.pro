TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += launcher password translate

launcher.file = apps/launcher/ToolboxLauncher.pro
password.file = apps/password_app/ToolboxPassword.pro
translate.file = apps/translate_app/ToolboxTranslate.pro
