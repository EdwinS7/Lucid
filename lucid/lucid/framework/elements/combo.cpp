#include "../framework.h"

void lucid_engine::ui::combo_box( const char* title, int* current_variable, std::vector< std::string > options ) {
	vec2_t combo_box_size = vec2_t( m_group_size[ m_group_id ].x - m_style->m_group_padding * 2, 16 );
	vec2_t text_size = g_renderer.get( )->get_text_size( g_renderer.get( )->m_defualt_font, title );
	bool pressed = false;
	static bool opened = false;

	// clamp our value so it doesnt go out of bounds.
	if ( *current_variable < 0 )
		*current_variable = 0;
	else if ( *current_variable >= options.size( ) )
		*current_variable = options.size( ) - 1;

	// check if mouse is hovering the bounds of the combo box.
	if ( g_input.get( )->mouse_hovering_rect( m_elements_pos, vec2_t( combo_box_size.x, text_size.y + combo_box_size.y ) ) ) {
		// check if mouse1 is pressed and change the state of the button.
		if ( g_input.get( )->is_key_pressed( VK_LBUTTON ) ) {
			pressed = true;

			// invert our opened variable when pressed that way we can open/close the combo box even while hovering.
			if ( pressed )
				opened = !opened;
		}

		// set this so we cannot drag our menu.
		m_hovering_element = true;
	}

	// check if check box is even visible and render if visible.
	if ( g_input.get( )->rect_clipping_rect( m_elements_pos, text_size,
											 m_window_pos[ m_window_id ] + m_group_pos[ m_group_id ] + vec2_t( 0, 16 ), m_group_size[ m_group_id ] - vec2_t( 0, 16 ) ) ) {
		g_renderer.get( )->filled_rounded_rectangle( m_elements_pos + vec2_t( 0, text_size.y ), combo_box_size, m_style->m_element_inactive, m_style->m_element_rounding );
		g_renderer.get( )->rounded_rectangle( m_elements_pos + vec2_t( 0, text_size.y ), combo_box_size, m_style->m_element_outline, m_style->m_element_rounding );

		// title of our combo box.
		g_renderer.get( )->text( g_renderer.get( )->m_defualt_font, title, m_elements_pos - vec2_t( 0, 4 ), m_style->m_text_active );

		// this will be where combo text is rendered.
		g_renderer.get( )->text( g_renderer.get( )->m_defualt_font, options.at( *current_variable ), m_elements_pos + vec2_t( 5, text_size.y + 2 ), m_style->m_text_inactive );
	}

	// if pressed then show our popup
	if ( opened ) {
		// loop through each item we have.
		for ( int i = 0; i < options.size( ); ++i ) {
			vec2_t popup_pos = m_elements_pos + vec2_t( 0, ( combo_box_size.y * ( i + 1 ) ) );
			// render each items popup option.
			popup( options.at( i ).c_str( ), popup_pos, color_t( 255, 255, 255 ) );
			// if our mouse is hovering over an option/variable we want to select then and only then do we want to edit its value.
			if ( g_input.get( )->mouse_hovering_rect( popup_pos, vec2_t( combo_box_size.x, text_size.y + combo_box_size.y ) ) ) {
				// check if mouse1 is pressed and if so change the value of our current variable.
				if ( g_input.get( )->is_key_pressed( VK_LBUTTON ) ) {
					// set our current variable.
					*current_variable = i;
					// close popup when one is chosen.
					// note: @edwn could make this not happen up to you really.
					opened = false;
				}
			}
		}

		// note: we want to do this after so we can still select our options otherwise it wont be saved properly.
		if ( g_input.get( )->is_key_pressed( VK_LBUTTON ) ) {
			opened = false;
		}
	}

	// apply new position.
	m_elements_pos += vec2_t( 0, text_size.y + combo_box_size.y + m_style->m_group_spacing );
}