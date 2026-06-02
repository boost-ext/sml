$(document).ready(function () {
    $('img[alt="IFRAME"]').each(function () {
      var id = $(this).attr('src');
      var html = '<iframe style="width: 100%;height: 850px;" src="' + id + '" frameborder="0" allowfullscreen></iframe>';
      $(this).replaceWith(html);
    });
});
$(document).ready(function () {
    $('img[alt="YOUTUBE"]').each(function () {
        var id = $(this).attr('src').split('/')[$(this).attr('src').split('/').length - 1];
        var video = '<iframe style="width: 100%;height: 450px;" src="https://www.youtube.com/embed/' + id + '?rel=0&amp;showinfo=0" frameborder="0" allowfullscreen></iframe>';
        $(this).replaceWith(video);
    });
});
// Open "Run on Compiler Explorer" links in a new browser tab.
$(document).ready(function () {
    $("a[href*='godbolt.org']").attr("target", "_blank").attr("rel", "noopener");
});
