<?php
    $end_time = microtime(true);
    $sql_time = sql::get_total_query_time();
    $page_time = $end_time - init::get_start_time();
    $php_time = $page_time - $sql_time;
    dlog::decho(" php time: " . $php_time);
    dlog::decho(" sql time: " . $sql_time);
    dlog::decho("page time: " . $page_time);
?>
<pre>
$_FILES <?= htmlentities(print_r($_FILES, true)); ?>
$_REQUEST <?= htmlentities(print_r($_REQUEST, true)); ?>
<?= htmlentities(dlog::get_decho_buffer()); ?>
</pre>
<pre id="javascriptDebug"></pre>
<script type="text/javascript">
    my.debugInitNode(document.getElementById('javascriptDebug'));
</script>