$(function() {
    $(':button').button();
    $('#setRelayOn').buttonset();
    var uncheckRelays = function() {
        $('#setRelayOn input').removeAttr('checked');
        $('#setRelayOn').buttonset('refresh');
    };
    uncheckRelays();

    $("input:radio[name=relay]").click(function(){
        relay = parseInt($(this).attr("value"));
        Sijax.request('setRelayOn',[relay]);
        uncheckRelays();
    });

    $('#setAllRelaysOff').click(function () {
        Sijax.request('setAllRelaysOff');
    });

});
