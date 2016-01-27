$(document).ready(function () {
    $('img[alt="HTML"]').each(function () {
        var id = $(this).attr('src');
        var html = '<iframe style="width: 100%;height: 850px;" src="' + id + '" frameborder="0" allowfullscreen></iframe>';
        $(this).replaceWith(html);
    });
});

