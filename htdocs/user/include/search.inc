<div class="smart-form"><!-- smart form !-->
<input type="hidden" name="action" value="controller_user.save" />
<input type="hidden" name="<?= session::requestkey(); ?>" value="<?= session::requestid(); ?>" />
<input type="hidden" name="user[userid]" value="<?= $model['user']['userid']['value'] ?>"/>
<fieldset>
        <div class="row">
            <section class="col col-6">
                <label class="label" for="userforename">First name</label>
                <label class="input<?php if ($model['user']['forename']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-user"></i>
                    <input type="text" name="user[forename]" id="userforename" value="<?= $model['user']['forename']['value']; ?>" placeholder="First name">
                    <?= template::error_message($model['user']['forename']['error']); ?>
                </label>
            </section>
            <section class="col col-6">
                <label class="label" for="useremail">Email</label>
                <label class="input<?php if ($model['user']['email']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-envelope-o"></i>
                    <input type="email" name="user[email]" id="useremail" value="<?= $model['user']['email']['value']; ?>" placeholder="Email">
                    <?= template::error_message($model['user']['email']['error']); ?>
                </label>
            </section>
        </div>
        <div class="row">
            <section class="col col-6">
                <label class="label" for="usersurname">Last name</label>
                <label class="input<?php if ($model['user']['surname']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-user"></i>
                    <input type="text" name="user[surname]" id="usersurname" value="<?= $model['user']['surname']['value']; ?>" placeholder="Last name">
                    <?= template::error_message($model['user']['surname']['error']); ?>
                </label>
            </section>
            <section class="col col-6">
                <label class="label" for="userdateofbirth">Date of birth</label>
                <label class="input<?php if ($model['user']['dateofbirth']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-user"></i>
                    <input class="dateofbirth" type="text" name="user[dateofbirth]" id="userdateofbirth" value="<?= $model['user']['dateofbirth']['value']; ?>" placeholder="Date of birth">
                    <?= template::error_message($model['user']['dateofbirth']['error']); ?>
                </label>
            </section>
        </div>
        <div class="row">
            <section class="col col-6">
                <label class="label" for="userhomephone">Phone</label>
                <label class="input<?php if ($model['user']['phone']['value'][0]['phone']['phone']['error']) echo " has-error"; ?>"> <i class="icon-prepend fa fa-phone"></i>
                    <input type="tel" name="user[phone][0][phone]" id="userhomephone" value="<?= $model['user']['phone']['value'][0]['phone']['phone']['value']; ?>" data-mask="(999) 999-9999" placeholder="Home phone">
                    <?= template::error_message($model['user']['phone']['value'][0]['phone']['phone']['error']); ?>
                </label>
                <input type="hidden" name="user[phone][0][phoneid]" value="<?= $model['user']['phone']['value'][0]['phone']['phoneid']['value']; ?>">
            </section>
        </div>
</fieldset>
<footer>
    <button type="submit" class="btn btn-primary">
        Find User
    </button>
</footer>
</div><!-- smart form !-->