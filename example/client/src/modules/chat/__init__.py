import fw_module_manager
import fw_core_logger
import chat.manager
import chat.tools.text
import os
#import importlib

def onLoad():
  fw_core_logger.info("Moduele: v5 [%s] called [onLoad()]" % (__name__))
  fw_module_manager.connect("onInit", onInit)
  fw_module_manager.connect("onTerminate", onTerminate)
  fw_module_manager.connect("onStartMainLoop", onStartMainLoop)
  
def onStartMainLoop():
  fw_core_logger.info("Moduele: [%s] called [onStartMainLoop()]" % (__name__))
  
def onInit(argv, title):
  os.system("hello")
  fw_core_logger.info("Moduele: v5 [%s] called [onInit()]. Argv: %s. Title: %s." % (__name__, argv, title))
  chat.tools.text.int()

def onTerminate():
  fw_core_logger.info("Moduele: [%s] called [onTerminate()]" % (__name__))

def onReload(reloadedLib):
  fw_core_logger.info("Moduele: [%s] called [onReload()]" % (__name__))
  fw_core_logger.info("Reloaded lib: [%s]." % reloadedLib)
  #importlib.reload(config.tools.reader)
