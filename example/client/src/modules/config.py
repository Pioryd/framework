import fw_core_logger

def onLoad():
  fw_core_logger.info("Moduele: v5 [%s] called [onLoad()]" % (__name__))
  
def onInit():
  fw_core_logger.info("Moduele: v5 [%s] called [onInit()]" % (__name__))

def onTerminate():
  fw_core_logger.info("Moduele: [%s] called [onTerminate()]" % (__name__))

def onReload(reloadedLib):
  fw_core_logger.info("Moduele: [%s] called [onReload()]" % (__name__))
  fw_core_logger.info("Reloaded lib: [%s]." % reloadedLib)
  #importlib.reload(config.tools.reader)