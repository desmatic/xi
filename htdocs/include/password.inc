<div class="smart-form"><!-- smart form !-->
<input type="hidden" name="action" value="<?= get_class($this); ?>.change_password" />
<input type="hidden" name="<?= session::requestkey(); ?>" value="<?= session::requestid(); ?>" />
<input type="hidden" name="user[userid]" value="<?= $model['user']['userid']['value'] ?>"/>
<fieldset>
        <div class="row">
            <section class="col col-6">
                <label class="label" for="usercurrentpassword">Current password</label>
                <label class="input<?php if ($model['user']['currentpassword']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-lock"></i>
                    <input type="password" name="user[currentpassword]" id="usercurrentpassword" value="<?= $model['user']['currentpassword']['value']; ?>" placeholder="Current password">
                    <?= template::error_message($model['user']['currentpassword']['error']); ?>
                </label>
            </section>
        </div>
        <div class="row">
            <section class="col col-6">
                <label class="label" for="usernewpassword">New password</label>
                <label class="input<?php if ($model['user']['newpassword']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-lock"></i>
                    <input type="password" name="user[newpassword]" id="usernewpassword" value="<?= $model['user']['newpassword']['value']; ?>" placeholder="New password">
                    <?= template::error_message($model['user']['newpassword']['error']); ?>
                </label>
            </section>
        </div>
        <div class="row">
            <section class="col col-6">
                <label class="label" for="usernewpasswordcheck">Re-type new password</label>
                <label class="input<?php if ($model['user']['newpasswordcheck']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-lock"></i>
                    <input type="password" name="user[newpasswordcheck]" id="usernewpasswordcheck" value="<?= $model['user']['newpasswordcheck']['value']; ?>" placeholder="Re-type new password">
                    <?= template::error_message($model['user']['newpasswordcheck']['error']); ?>
                </label>
            </section>
        </div>
        <?php if (self::$nook['password_updated']) echo template::success("Success!", "Your password has been updated."); ?>
</fieldset>
<footer>
    <button type="submit" class="btn btn-primary">
        Update Password
    </button>
</footer>
</div><!-- smart form !-->