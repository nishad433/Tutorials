$(document).ready(function () {
  $(".burger").click(function (event) {
    // Toggle nav-link
    $(".nav-links").toggleClass("nav-links-active");
    $(".burger").toggleClass("toggle");

    // Animate nav-link items
    $(".nav-links")
      .children("li")
      .each(function (index, item) {
        if ($(item).css("animation-name") === "none") {
          delay = index / 7 + 0.5;
          //animationVals = "nav-link-animate 0.5s ease forwards " + delay;
          //$(item).css("animation", animationVals);

        $(item).css("animation-name","nav-link-animate");
        $(item).css("animation-duration","0.5s");
        $(item).css("animation-timing-function","ease");
        $(item).css("animation-fill-mode","forwards");
        $(item).css("animation-delay",delay+"s");

        } else {
          $(item).css("animation", "");
          console.log($(item).css("animation"));
        }
      });
  });
});
