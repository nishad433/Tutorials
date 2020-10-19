// Calls when DOM is ready, similiar to JQuery document.ready()

document.addEventListener("DOMContentLoaded", function (ev) {
  initJS(ev);
});

function initJS(ev) {
  const menuBtn = document.querySelector("div");
  var menuOpen = false;

  menuBtn.addEventListener("click", function (ev) {

    if(menuOpen){
        menuBtn.classList.remove('open');
        menuOpen=false
    }else{
        menuBtn.classList.add('open');
        menuOpen=true;
    }

  });
}
