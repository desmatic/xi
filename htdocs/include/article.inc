<div class="well">
    <div class="pull-left">
        <a href="/"><img src="/thumb.html?md5sum=<?= $model['user_md5sum']; ?>&amp;bytes=<?= $model['user_bytes']; ?>&amp;size=thumb128x128" width="48" alt="avatar" class="img-rounded rpadded" /></a>
    </div>
    <?php if (session::agent_can_do(cando::ADD_ARTICLE_CONTROLLER_ROOT, $model['userid']) || session::agent_can_do(cando::EDIT_ARTICLE_CONTROLLER_ROOT)) : ?>
        <div class="btn-group pull-right">
            <a href="javascript:void(0);" data-toggle="dropdown" class="btn btn-default btn-xs dropdown-toggle"><span class="caret single">&nbsp;</span></a>
            <ul class="dropdown-menu text-left">
                <?php if (session::agent_can_do(cando::PIN_ARTICLE_CONTROLLER_ROOT)) : ?>
                    <?php
                        $pin = "Unpin";
                        if (empty($model['datepinned'])) {
                            $pin = "Pin";
                        }
                    ?>
                    <li>
                        <a href="?action=controller_article.pin&amp;article[articleid]=<?= $model['articleid']; ?>&amp;pin=<?= $pin; ?>"><?= $pin; ?> this post</a>
                    </li>
                <?php endif; ?>
                <li>
                    <a href="/article/article.html?article[articleid]=<?= $model['articleid']; ?>">Edit this post</a>
                </li>
                <li>
                    <a href="?action=controller_article.pause&amp;article[articleid]=<?= $model['articleid']; ?>&amp;pause=pause">Pause this post</a>
                </li>
                <li>
                    <a href="?action=controller_article.delete&amp;article[articleid]=<?= $model['articleid']; ?>&amp;delete=delete">Delete this post</a>
                </li>
            </ul>
        </div>
    <?php endif; ?>
    <small><?= session::local_calendar_day($model['datetimestart']) . " at " . session::local_time($model['datetimestart']); ?></small>
    <?php if (!empty($model['datepinned'])) : ?>
        <a href="javascript:void(0);" rel="tooltip" data-placement="top" data-original-title="<?= "Pinned " . session::local_calendar_day($model['datepinned']) . " at " . session::local_time($model['datepinned']); ?>"><i class="icon-prepend fa fa-thumb-tack"></i></a>
    <?php endif; ?>
    <h4><?= $model['nickname']; ?> <small><?= $model['email']; ?> </small></h4>
    <br />
    <?= $model['article']; ?>
    <?php if (!empty($model['md5sum']) && !empty($model['bytes'])) : ?>
        <img class="img-rounded img-responsive" src="/thumb.html?md5sum=<?= $model['md5sum']; ?>&amp;bytes=<?= $model['bytes']; ?>&amp;size=thumb504" alt="">
        <br />
    <?php endif; ?>
    <?php if (!empty($model['question']) && ($answers = article::parse_answers($model['answers']))) : ?>
        <p><?= htmlentities($model['question']); ?></p>
        <form id="formpoll">
            <div class="radio">
                <ul class="liststylenone">
                    <?php foreach ($answers as $answer) : ?>
                    <li><label><input type="radio" name="answer" value="<?= htmlentities($answer); ?>" <?php if ($answer == $model['answer']) echo "checked=\"checked\"";?>/> <?= htmlentities($answer); ?></label></li>
                    <?php endforeach; ?>
                </ul>
            </div>
            <div id="formpollresults">
            </div>
            <footer>
                <button type="submit" class="btn btn-primary" onclick="my.ajax.formInnerHtml('/ajax.html', { 'action':'controller_article.savepoll','article[articleid]':<?= $model['articleid']; ?> }, document.getElementById('formpoll'), document.getElementById('formpollresults')); return false;">
                    Vote Now
                </button>
            </footer>
        </form>
    <?php endif; ?>
</div>