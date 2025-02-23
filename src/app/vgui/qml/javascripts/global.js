.pragma library

/**
 * @brief setTimeout 定时器
 * @param callback
 * @param timeout
 */
function setTimeout(callback,timeout, repeated, item) {

  let timer = Qt.createQmlObject("import QtQuick 2.12; Timer {}", item);
  timer.interval = timeout;
  timer.repeat = repeated;
  timer.triggered.connect(callback);
  timer.start();
  return timer;
}

function llog()
{
    console.log("llog")
}

function obejctEmpty(obj)
{
    return Object.keys(obj).length === 0;
//    return Object.getOwnPropertyNames(obj).length === 0;
//    return Object.entries(obj).length === 0;
}
