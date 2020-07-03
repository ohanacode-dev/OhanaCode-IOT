function add_url()
{
    var x;
    var stream_url = prompt("Please input new stream url:","");
    if (stream_url != null){
        redirect_url = '/?action=add&url="' + encodeURI(stream_url) + '"';
        window.location.href = redirect_url;

    }
}

