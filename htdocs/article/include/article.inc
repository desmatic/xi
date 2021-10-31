<?php 
    if (isset(self::$nook['controller_article.save'])) {
        if (self::$nook['controller_article.save']) {
            echo template::success("Success!", "Your article has been updated."); 
        } else {
            echo template::danger("Oops!", "Your article couldn't be updated. Please check you've entered all the required fields."); 
        }
    }
?>
<form enctype="multipart/form-data" method="post">
<div class="smart-form"><!-- smart form !-->
<input type="hidden" name="MAX_FILE_SIZE" value="4000000" />
<input type="hidden" name="action" value="<?= get_class($this); ?>.save" />
<input type="hidden" name="<?= session::requestkey(); ?>" value="<?= session::requestid(); ?>" />
<input type="hidden" name="article[articleid]" value="<?= $model['article']['articleid']['value'] ?>"/>
<header>
    Article Scope
</header>
<fieldset>
    <div class="row">
        <section class="col col-3">
            <label class="label" for="articledatestart">*Start Date</label>
            <label class="input<?php if ($model['article']['datestart']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-calendar-o"></i>
                <input class="date" type="text" name="article[datestart]" id="articledatestart" value="<?= $model['article']['datestart']['value']; ?>" placeholder="Start Date">
                <b class="tooltip tooltip-top-right">Date when the article should appear.</b>
                <?= template::error_message($model['article']['datestart']['error']); ?>
            </label>
        </section>
        <section class="col col-2">
            <label class="label" for="articletimestart">Start Time</label>
            <label class="input<?php if ($model['article']['timestart']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-clock-o"></i>
                <input class="clockpicker" type="text" name="article[timestart]" id="articletimestartt" value="<?= $model['article']['timestart']['value']; ?>" placeholder="00:00">
                <?= template::error_message($model['article']['timestart']['error']); ?>
            </label>
        </section>
        <section class="col col-3">
            <label class="label" for="articledateend">End Date*</label>
            <label class="input<?php if ($model['article']['dateend']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-calendar-o"></i>
                <input class="date" type="text" name="article[dateend]" id="articledateend" value="<?= $model['article']['dateend']['value']; ?>" placeholder="End Date">
                <b class="tooltip tooltip-top-right">Date when the article should disappear.</b>
                <?= template::error_message($model['article']['dateend']['error']); ?>
            </label>
        </section>
        <section class="col col-2">
            <label class="label" for="articletimeend">End Time</label>
            <label class="input<?php if ($model['article']['timeend']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-clock-o"></i>
                <input class="clockpicker" type="text" name="article[timeend]" id="articletimeend" value="<?= $model['article']['timeend']['value']; ?>" placeholder="23:59">
                <?= template::error_message($model['article']['timeend']['error']); ?>
            </label>
        </section>
    </div>
    <section>
        <label class="label">Display Options</label>
        <div class="row">
            <div class="col col-3">
                <label class="checkbox">
                    <?php if ($model['article']['datepaused']['value']) : ?>
                        <input type="checkbox" name="article[pause]" value="unpause"/><i></i>Unpause Article
                    <?php else : ?>
                        <input type="checkbox" name="article[pause]" value="pause"/><i></i>Pause Article
                    <?php endif; ?>
                </label>
                <label class="checkbox">
                    <?php if ($model['article']['datepinned']['value']) : ?>
                        <input type="checkbox" name="article[pin]" value="unpin"/><i></i>Unpin Article
                    <?php else : ?>
                        <input type="checkbox" name="article[pin]" value="pin"/><i></i>Pin Article
                    <?php endif; ?>
                </label>
            </div>
        </div>
    </section>
    <section>
        <label class="label">*Companies</label>
        <div class="row">
            <?php 
                $output = "";
                $checkboxes = "";
                $c = new company();
                $rows = $c->lookup();
                $i = 0;
                $j = 0;
                foreach($rows as $row) {
                    $checked = "";
                    $onclick = "";
                    if ($i < 3) {
                        foreach($model['article']['article_company']['value'] as $article_company) {
                            if ($article_company['article_company']['companyid']['value'] == $row['id']) {
                                if (empty($article_company['article_company']['datedeleted']['value'])) {
                                    $checked = "checked=\"checked\"";
                                }
                                $checkboxes .= "<input type=\"hidden\" name=\"article[article_company][{$j}][article_company_id]\" value=\"{$article_company['article_company']['article_company_id']['value']}\">\n";
                                $checkboxes .= "<input type=\"hidden\" name=\"article[article_company][{$j}][datedeleted]\" value=\"{$article_company['article_company']['datedeleted']['value']}\" id=\"companycompanyid{$row['id']}\">\n";
                                $onclick = "onclick=\"var e = document.getElementById('companycompanyid{$row['id']}'); if (this.checked) { e.value='null'; } else { e.value='sysdate'; }\"";
                            }
                        }
                        $checkboxes .= "<label class=\"checkbox\"><input type=\"checkbox\" name=\"article[article_company][$j][companyid]\" value=\"{$row['id']}\" {$checked} {$onclick}/><i></i>{$row['name']}</label>\n";
                    } else {
                        $output .= "<div class=\"col col-3\">\n{$checkboxes}</div>\n";
                        $checkboxes = "";
                        $i = 0;
                    }
                    $i++;
                    $j++;
                }
                if ($checkboxes) {
                    $output .= "<div class=\"col col-3\">\n{$checkboxes}</div>\n";
                }
                
                echo $output;
            ?>
        </div>
    </section>
</fieldset>
<header>
    Article Details
</header>
<fieldset>
    <div class="row">
        <section class="col col-6">
            <label class="label" for="articletitle">*Title</label>
            <label class="input<?php if ($model['article']['title']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-file-text-o"></i>
                <input type="text" name="article[title]" id="articletitle" value="<?= $model['article']['title']['value']; ?>" placeholder="Title">
                <b class="tooltip tooltip-bottom-right">The article title needs to be unique. Tip: add a date or time, like Xmas 2015 for example.</b>
                <?= template::error_message($model['article']['title']['error']); ?>
            </label>
        </section>
    </div>
    <section>
            <label class="label">Text</label>
            <label class="textarea">
                <textarea name="article[article]" id="articlearticle" rows="3" class="custom-scroll tinymce"><?= $model['article']['article']['value']; ?></textarea> 
            </label>
    </section>
    <section>
        <label class="label">Image</label>
        <div class="input input-file">
            <span class="button"><input type="file" id="articlefile" name="file" onchange="this.parentNode.nextSibling.value = this.value">Browse</span><input type="text" placeholder="896x504" readonly="">
            <b class="tooltip tooltip-bottom-left">For best compression results, use images with a 16:9 ratio that are 896x504 or larger.</b>
        </div>
    </section>
</fieldset>
<?php if (!empty($model['article']['user_filedrive']['value'][0])) : ?>
<div>
    <img class="img-rounded img-responsive" src="/thumb.html?md5sum=<?= $model['article']['user_filedrive']['value'][0]['user_filedrive']['filedrive']['value'][0]['filedrive']['md5sum']['value']; ?>&amp;bytes=<?= $model['article']['user_filedrive']['value'][0]['user_filedrive']['filedrive']['value'][0]['filedrive']['bytes']['value']; ?>&amp;size=thumb504" alt="">
</div>
<?php endif; ?>
<header>
    Article Poll
</header>
<fieldset>
    <section>
            <label class="label">Poll Question</label>
            <label class="textarea">
                <textarea name="article[question]" id="articlequestion" class="custom-scroll"><?= $model['article']['question']['value']; ?></textarea> 
                <b class="tooltip tooltip-bottom-left">You can add a poll to your post by asking a question here.</b>
            </label>
    </section>
    <section>
        <label class="label">Poll Answers (one choice per line)</label>
        <label class="textarea textarea-resizable">
            <textarea name="article[answers]" id="articleanswers" rows="4" class="custom-scroll"><?= $model['article']['answers']['value']; ?></textarea>
            <b class="tooltip tooltip-bottom-left">Option 1</br >Option 2</br >Option 3</br >...</br >Option X</b>
        </label>
    </section>
</fieldset>
<footer>
    <button type="submit" class="btn btn-primary">
        Save
    </button>
    <a class="btn btn-primary" href="?action=controller_article.delete&amp;article[articleid]=<?= $model['article']['articleid']['value']; ?>&amp;delete=delete">Delete</a>
</footer>
</div><!-- smart form !-->
</form>